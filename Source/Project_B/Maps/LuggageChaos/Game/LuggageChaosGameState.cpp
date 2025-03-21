// Fill out your copyright notice in the Description page of Project Settings.


#include "LuggageChaosGameState.h"

#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "Project_B/Maps/LuggageChaos/Widget/LuggageScoreWidget.h"
#include "Project_B/Utilities/LogMacro.h"


void ALuggageChaosGameState::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		FTimerHandle OnStartTimerHandle;
		GetWorld()->GetTimerManager().SetTimer(OnStartTimerHandle, this, &ALuggageChaosGameState::Net_AddWidget,2.0f,false);
	}

	//bReplicates = true;
}

void ALuggageChaosGameState::AddScore(ETeamType team, const uint8 point)
{
	if (team == ETeamType::Blue)
	{
		BluePoint += point;
		//LOG_SCREEN("블루팀 스코어: %d", BluePoint);
		
		ScoreWidget->UpdateBlueScore(BluePoint);
		
		if (BluePoint >= MaxPoint)
		{
			Win(ETeamType::Blue);
		}
		
	}
	else
	{
		RedPoint += point;
		//LOG_SCREEN("레드팀 스코어: %d", RedPoint);

		ScoreWidget->UpdateRedScore(RedPoint);
		
		if (RedPoint >= MaxPoint)
		{
			Win(ETeamType::Red);
		}
	}
}

void ALuggageChaosGameState::Net_AddWidget_Implementation()
{
	for (APlayerState* PS : PlayerArray)
	{
		APlayerController* PC = Cast<APlayerController>(PS->GetPlayerController());
		if (PC && PC->IsLocalController())
		{
			ScoreWidget = CreateWidget<ULuggageScoreWidget>(PC, ScoreWidgetClass);
			if (ScoreWidget)
			{
				ScoreWidget->AddToViewport();
			}
		}
	}
}

void ALuggageChaosGameState::Win(ETeamType team)
{
	if (team == ETeamType::Blue)
	{
		LOG_SCREEN("블루팀 승리");
	}
	else
	{
		LOG_SCREEN("레드팀 승리");
	}
}
