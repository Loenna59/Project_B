#include "LuggageChaosGameState.h"

#include <rapidjson/reader.h>

#include "LuggageChaosGameMode.h"
#include "LuggagePlayerState.h"

#include "Blueprint/UserWidget.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "Project_B/Maps/LobbyMap/BanimalsGameInstance.h"
#include "Project_B/Maps/LuggageChaos/Widget/LuggageScoreWidget.h"
#include "Project_B/Maps/Base/Widget/GameReadyWidget.h"
#include "Project_B/Maps/Base/Widget/GameEndWidget.h"
#include "Project_B/Maps/Podium/WinnerPrize.h"
#include "Project_B/Utilities/LogMacro.h"


ALuggageChaosGameState::ALuggageChaosGameState()
{
	LatenActionInfo. CallbackTarget = this;
	LatenActionInfo.ExecutionFunction = FName("OnLevelLoadComplete");
	LatenActionInfo.UUID = 1; 
	LatenActionInfo.Linkage = 0;    
}

void ALuggageChaosGameState::BeginPlay()
{
	Super::BeginPlay();
	GameReady();
}

void ALuggageChaosGameState::GameReady()
{
	if (HasAuthority())
	{
		UBanimalsGameInstance* gi = Cast<UBanimalsGameInstance>(GetWorld()->GetGameInstance());

		//TODO: gi로부터 플레이어 정보 가져오기
		PlayersInfo= gi->GetPlayerInfo();
	
		//PlayersInfo = DummyPlayersInfo();
		
		FTimerHandle OnStartTimerHandle;
		GetWorld()->GetTimerManager().SetTimer(OnStartTimerHandle, this, &ALuggageChaosGameState::GameStart,ReadyTime,false);
		
		InitSpawnPoint();
	}

	APlayerController* pc = GetWorld()->GetFirstPlayerController();
	
	InitUI(pc);
}

void ALuggageChaosGameState::InitPlayerLoc(APawn* pawn)
{
	if (HasAuthority() == false)
	{
		return;
	}
	
	//FString dummyKey = FString::FromInt(dummyIdx);

	const FUniqueNetIdRepl& NetIdRepl = pawn->GetPlayerState<APlayerState>()->GetUniqueId();
	FString Key;
	if (NetIdRepl.IsValid())
	{
		TSharedPtr<const FUniqueNetId> NetId = NetIdRepl.GetUniqueNetId();
		 Key = NetId->ToString();
	}
	
	if (FPlayerInfo* Info = PlayersInfo.Find(Key))
	{
		if (Info->Team == ETeamType::Blue)
		{
			LOG_PRINT(TEXT("나는 파랑팀"));
			pawn->SetActorLocation(BlueSpawnPoints[blueIdx]->GetActorLocation());
			pawn->SetActorRotation(BlueSpawnPoints[blueIdx]->GetActorRotation());
			++blueIdx;
		}
		else
		{
			LOG_PRINT(TEXT("나는 레드팀"));
			pawn->SetActorLocation(RedSpawnPoints[redIdx]->GetActorLocation());
			pawn->SetActorRotation(RedSpawnPoints[blueIdx]->GetActorRotation());
			++redIdx;
		}
	}
	else
	{
		LOG_ERROR(this,TEXT("존재하지 않는 Key"));
	}

	//++dummyIdx;
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
	if (HasAuthority())
	{
		Net_GameEnd();
	}
}

void ALuggageChaosGameState::Net_GameEnd_Implementation()
{
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), SlowTime);
	
	UGameplayStatics::LoadStreamLevel(GetWorld(), FName("LV_Poidum"),false,false,LatenActionInfo);
}

void ALuggageChaosGameState::OnLevelLoadComplete(UWorld* loadedWorld)
{
	// LOG_SCREEN("비동기 레벨 로드 완료");
	// if (HasAuthority())
	// {
	// 	LevelLoadComplete();
	// }
	// Server_LevelLoadComplete(); 
}

void ALuggageChaosGameState::LevelLoadComplete()
{
	++LoadComplete;

	LOG_PRINT(TEXT("로드완료된 인원: %d"), LoadComplete);
	LOG_PRINT(TEXT("총 플레이어 수: %d"), PlayerArray.Num());
	
	if (LoadComplete >= PlayerArray.Num())
	{
		ChangeLevelPodium();
	}
}

void ALuggageChaosGameState::Server_LevelLoadComplete_Implementation()
{
	LevelLoadComplete();
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
		GameEnd();
		return;
	}
	
	const FUniqueNetIdRepl& NetIdRepl = GetWorld()->GetFirstPlayerController()->GetPlayerState<APlayerState>()->GetUniqueId();

	TSharedPtr<const FUniqueNetId> NetId = nullptr;
	if (NetIdRepl.IsValid())
	{
		NetId = NetIdRepl.GetUniqueNetId();
	}
	else
	{
		LOG_ERROR(this,TEXT("NetID Null"));
		return;
	}
	
	if (FPlayerInfo* info = PlayersInfo.Find(NetId->ToString()))
	{
		myTeam = info->Team;
	}

	//TODO: 테스트 더미
	if (HasAuthority())
	{
		myTeam = ETeamType::Blue;
	}
	else
	{
		myTeam = ETeamType::Red;
	}
	
	if (myTeam == winner)
	{
		APlayerController* pc = GetWorld()->GetFirstPlayerController();
		AddWinPrize(pc);
		GameEndWidget->ShowVictory();
		LOG_SCREEN("WIN");
	}
	else
	{
		GameEndWidget->ShowDefeat();
		LOG_SCREEN("LOSE");
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