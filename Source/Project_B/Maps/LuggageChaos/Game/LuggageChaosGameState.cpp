// Fill out your copyright notice in the Description page of Project Settings.


#include "LuggageChaosGameState.h"

#include "LuggageChaosGameMode.h"
#include "LuggagePlayerState.h"
#include "MovieSceneTracksComponentTypes.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "Project_B/Maps/LuggageChaos/Widget/LuggageScoreWidget.h"
#include "Project_B/Maps/LuggageChaos/Widget/GameEndWidget.h"
#include "Project_B/Maps/Podium/WinnerPrize.h"
#include "Project_B/Utilities/LogMacro.h"


void ALuggageChaosGameState::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		LOG_SCREEN("READY..");
	
		FTimerHandle OnStartTimerHandle;
		
		GetWorld()->GetTimerManager().SetTimer(OnStartTimerHandle, this, &ALuggageChaosGameState::GameStart,ReadyTime,false);

		TrueEndTime = EndTime * SlowTime;
	}
}

void ALuggageChaosGameState::GameStart()
{
	LOG_SCREEN("START!");
	GetWorld()->GetTimerManager().SetTimer(GameTimerHandle, this, &ALuggageChaosGameState::TimeOut,GameTime,false);

	Net_InitUI();
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

void ALuggageChaosGameState::Net_InitUI_Implementation()
{
	for (APlayerState* ps : PlayerArray)
	{
		APlayerController* pc = Cast<APlayerController>(ps->GetPlayerController());
		if (pc && pc->IsLocalController())
		{
			ScoreWidget = CreateWidget<ULuggageScoreWidget>(pc, ScoreWidgetClass);
			
			if (ScoreWidget)
			{
				ScoreWidget->AddToViewport();
			}

			GameEndWidget = CreateWidget<UGameEndWidget>(pc, GameEndWidgetClass);
			
			if (GameEndWidget)
			{
				GameEndWidget->AddToViewport();
			}
		}
	}

	//TODO: 테스트용
	AddScore(ETeamType::Red, 4);
	
	APlayerController* pc = GetWorld()->GetFirstPlayerController();

	APlayerState* ps = pc->PlayerState;
	ALuggagePlayerState* psLug = Cast<ALuggagePlayerState>(ps);
	
	if (psLug)
	{
		if (HasAuthority())
		{
			psLug->SetTeamType(ETeamType::Blue);
			LOG_SCREEN_MY(2.0f,FColor::Blue,"나는 파랑팀");
		}
		else
		{
			psLug->SetTeamType(ETeamType::Red);
			LOG_SCREEN_MY(2.0f,FColor::Red,"나는 빨강팀");
		}
	}
}

void ALuggageChaosGameState::GameEnd()
{
	if (HasAuthority())
	{
		FTimerHandle OnEndTimerHandle;
	
		GetWorld()->GetTimerManager().SetTimer(OnEndTimerHandle, this, &ALuggageChaosGameState::ChangeLevelPodium, TrueEndTime, false);
		Net_GameEnd();
	}
}

void ALuggageChaosGameState::Net_GameEnd_Implementation()
{
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), SlowTime);
}

void ALuggageChaosGameState::ChangeLevelPodium()
{
	LOG_SCREEN("레벨 전환");
	GetWorld()->ServerTravel(TEXT("/Game/Maps/Podium/LV_Poidum?listen"));
}

void ALuggageChaosGameState::TimeOut()
{
	Net_JudgeWinner();
}

void ALuggageChaosGameState::Win(ETeamType team)
{
	WinnerTeam = team;

	if (team == ETeamType::None)
	{
		GameEndWidget->ShowDraw();
		GameEnd();
		return;
	}
	
	APlayerController* pc = GetWorld()->GetFirstPlayerController();

	APlayerState* ps = pc->PlayerState;
	ALuggagePlayerState* psLug = Cast<ALuggagePlayerState>(ps);


	//TODO: 게임인스턴스에서 정보 가져오는 것으로 수정
	if (psLug)
	{
		if (psLug->GetTeamType() == team)
		{
			AddWinPrize(pc);
			GameEndWidget->ShowVictory();
			LOG_SCREEN("WIN");
		}
		else
		{
			GameEndWidget->ShowDefeat();
			LOG_SCREEN("LOSE");
		}
	}
	
	GameEnd();
}

void ALuggageChaosGameState::AddWinPrize(APlayerController* pc)
{
	AWinnerPrize* prize = GetWorld()->SpawnActor<AWinnerPrize>(WinnerPrizeClass, pc->GetCharacter()->GetTransform());
	if (prize)
	{
		prize->AttachToComponent(pc->GetCharacter()->GetMesh(), 
								 FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	}
}

void ALuggageChaosGameState::Net_JudgeWinner_Implementation()
{
	if (RedPoint == BluePoint)
	{
		//LOG_SCREEN("DRAW GAME");
		Win(ETeamType::None);
	}

	else if (RedPoint > BluePoint)
	{
		//LOG_SCREEN("WINNER: Red");
		Win(ETeamType::Red);
	}
	else
	{
		//LOG_SCREEN("WINNER: Blue");
		Win(ETeamType::Blue);
	}
}
