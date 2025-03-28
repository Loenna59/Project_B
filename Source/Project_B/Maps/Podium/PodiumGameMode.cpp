// Fill out your copyright notice in the Description page of Project Settings.


#include "PodiumGameMode.h"
#include "PodiumGameState.h"
#include "GameFramework/PlayerState.h"
#include "Project_B/Utilities/LogMacro.h"

void APodiumGameMode::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);
	APodiumGameState* gs = Cast<APodiumGameState>(GetWorld()->GetGameState());
	if (gs)
	{
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, [NewPlayer, this]()
		{
			if (NewPlayer && NewPlayer->GetPawn())
			{
				APodiumGameState* gs = Cast<APodiumGameState>(GetWorld()->GetGameState());
				if (gs)
				{
					gs->InitPlayerLoc(NewPlayer->GetPawn());
					gs->InitPodiumCamera(NewPlayer->GetPlayerState<APlayerState>()->GetPlayerController());
				}
			}
		}, BeginDelay, false); 
	}
	else
	{
		LOG_ERROR(this,TEXT("올바른 게임스테이트 미할당"));
	}
}
