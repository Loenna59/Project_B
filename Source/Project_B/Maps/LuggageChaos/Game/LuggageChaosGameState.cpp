#include "LuggageChaosGameState.h"

#include <rapidjson/reader.h>

#include "LuggageChaosGameMode.h"
#include "LuggagePlayerState.h"

#include "Blueprint/UserWidget.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/LowLevelTestAdapter.h"
#include "Net/UnrealNetwork.h"
#include "Project_B/Maps/LobbyMap/BanimalsGameInstance.h"
#include "Project_B/Maps/LuggageChaos/Widget/LuggageScoreWidget.h"
#include "Project_B/Maps/Base/Widget/GameReadyWidget.h"
#include "Project_B/Maps/Base/Widget/GameEndWidget.h"
#include "Project_B/Maps/Podium/WinnerPrize.h"
#include "Project_B/Utilities/LogMacro.h"


ALuggageChaosGameState::ALuggageChaosGameState()
{
}

void ALuggageChaosGameState::BeginPlay()
{
	Super::BeginPlay();
	GameReady();
}

void ALuggageChaosGameState::GameReady()
{
	UBanimalsGameInstance* gi = Cast<UBanimalsGameInstance>(GetWorld()->GetGameInstance());
	
	if (isDummyPlayerInfo)
	{
		PlayersInfo = DummyPlayersInfo();
		MyKey = FString::FromInt(dummyIdx);
		UE_LOG(LogTemp,Warning,TEXT("나의 키: %s"), *MyKey);
	}
	else
	{
		PlayersInfo= gi->GetPlayerInfo();

		const FUniqueNetIdRepl& NetIdRepl = GetWorld()->GetFirstPlayerController()->GetPlayerState<APlayerState>()->GetUniqueId();

		if (NetIdRepl.IsValid())
		{
			TSharedPtr<const FUniqueNetId> NetId = NetIdRepl.GetUniqueNetId();
			MyKey = NetId->ToString();
			UE_LOG(LogTemp,Error,TEXT("나의 키: %s"), *MyKey);

			FPlayerInfo* Info = PlayersInfo.Find(MyKey);
			if (Info->Team == ETeamType::Blue)
			{
				UE_LOG(LogTemp,Error,TEXT("나의 팀: Blue"));
			}
			else
			{
				UE_LOG(LogTemp,Error,TEXT("나의 팀: Red"));
			}
			
		}
	}
	
	if (HasAuthority())
	{
		FTimerHandle OnStartTimerHandle;
		GetWorld()->GetTimerManager().SetTimer(OnStartTimerHandle, this, &ALuggageChaosGameState::GameStart,ReadyTime,false);
		
		InitSpawnPoint();
	}

	APlayerController* pc = GetWorld()->GetFirstPlayerController();
	
	InitUI(pc);
}

void ALuggageChaosGameState::InitPlayerLoc(APawn* pawn,FString key)
{
	UE_LOG(LogTemp,Error,TEXT("키: %s 를 가진 플레이어는"), *key);
	if (FPlayerInfo* Info = PlayersInfo.Find(key))
	{
		if (Info->Team == ETeamType::Blue)
		{
			LOG_PRINT(TEXT("저는 파랑팀"));
			pawn->SetActorLocation(BlueSpawnPoints[blueIdx]->GetActorLocation());
			pawn->SetActorRotation(BlueSpawnPoints[blueIdx]->GetActorRotation());
			++blueIdx;
		}
		else
		{
			LOG_PRINT(TEXT("저는 레드팀"));
			pawn->SetActorLocation(RedSpawnPoints[redIdx]->GetActorLocation());
			pawn->SetActorRotation(RedSpawnPoints[blueIdx]->GetActorRotation());
			++redIdx;
		}
	}
	else
	{
		LOG_ERROR(this,TEXT("존재하지 않는 Key"));
	}
}

void ALuggageChaosGameState::InitUI(APlayerController* pc)
{
	ScoreWidget = CreateWidget<ULuggageScoreWidget>(pc, ScoreWidgetClass);
	if (ScoreWidget)
	{
		ScoreWidget->AddToViewport();
	}
	
	GameReadyWidget = CreateWidget<UGameReadyWidget>(pc, ReadyWidgetClass);
	if (GameReadyWidget)
	{
		GameReadyWidget->AddToViewport();
	}
	
	GameEndWidget = CreateWidget<UGameEndWidget>(pc, GameEndWidgetClass);
	if (GameEndWidget)
	{
		GameEndWidget->AddToViewport();
	}

	//TODO: 테스트
	if (isAddScoreBlue)
	{
		AddScore(ETeamType::Blue, 4);
	}
	if (isAddScoreRed)
	{
		AddScore(ETeamType::Red, 4);
	}
	
}

void ALuggageChaosGameState::InitSpawnPoint()
{
	TArray<AActor*> actorsB;
	TArray<AActor*> actorsR;
	
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("BlueSpawnPoint"), actorsB);

	for (AActor* actor : actorsB)
	{
		BlueSpawnPoints.Add(actor);
	}
	
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("RedSpawnPoint"), actorsR);
	
	for (AActor* actor : actorsR)
	{
		RedSpawnPoints.Add(actor);
	}
}

void ALuggageChaosGameState::GameStart()
{
	Net_GameStart();
	
	GetWorld()->GetTimerManager().SetTimer(GameTimerHandle, this, &ALuggageChaosGameState::TimeOut,GameTime,false);
}

void ALuggageChaosGameState::Net_GameStart_Implementation()
{
	if (GameReadyWidget)
	{
		GameReadyWidget->PlayAnimLoadComplete();
	}
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

void ALuggageChaosGameState::GameEnd()
{
	FTimerHandle OnEndTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(OnEndTimerHandle, this, &ALuggageChaosGameState::ChangeLevelPodium,EndTime*SlowTime,false);
	
	Net_GameEnd();
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

void ALuggageChaosGameState::Win(ETeamType winner)
{
	WinnerTeam = winner;
	ETeamType myTeam = ETeamType::None;

	if (winner == ETeamType::None)
	{
		GameEndWidget->ShowDraw();
		
		if (HasAuthority())
		{
			GameEnd();
		}
		return;
	}

	FPlayerInfo* myInfo = PlayersInfo.Find(MyKey);
	
	if (myInfo == nullptr)
	{
		LOG_ERROR(this,TEXT("나의 키: %s, 키가 없어욤"),*MyKey);
		return;
	}
	
	if (myInfo->Team == winner)
	{
		GameEndWidget->ShowVictory();
		LOG_SCREEN("WIN");
		
		APlayerController* pc = GetWorld()->GetFirstPlayerController();
		AddWinPrize(pc);
		myInfo->bIsWin = true;
	}
	else
	{
		GameEndWidget->ShowDefeat();
		LOG_SCREEN("LOSE");
	}
	
	if (HasAuthority())
	{
		GameEnd();
	}
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

TMap<FString,FPlayerInfo> ALuggageChaosGameState::DummyPlayersInfo()
{
	TMap<FString, FPlayerInfo> dummyPlayers;
	
	FPlayerInfo Player0;
	Player0.PlayerID = 0;
	Player0.PlayerName = TEXT("Player0");
	Player0.Team = ETeamType::Blue; 
	Player0.bIsReady = true;
	Player0.bIsAlive = true;
	Player0.bIsWin = false;
	dummyPlayers.Add(TEXT("0"), Player0);

	FPlayerInfo Player1;
	Player1.PlayerID = 1;
	Player1.PlayerName = TEXT("Player1");
	Player1.Team = ETeamType::Red;
	Player1.bIsReady = true;
	Player1.bIsAlive = true;
	Player1.bIsWin = false;
	dummyPlayers.Add(TEXT("1"), Player1);

	FPlayerInfo Player2;
	Player2.PlayerID = 2;
	Player2.PlayerName = TEXT("Player3");
	Player2.Team = ETeamType::Blue;
	Player2.bIsReady = true;
	Player2.bIsAlive = true;
	Player2.bIsWin = false;
	dummyPlayers.Add(TEXT("2"), Player2);
	
	return dummyPlayers;
}