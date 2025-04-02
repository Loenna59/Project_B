#include "LuggageChaosGameState.h"

#include "LuggageChaosGameMode.h"
#include "LuggagePlayerState.h"

#include "Blueprint/UserWidget.h"
#include "Camera/CameraComponent.h"
#include "Character/BaseCharacter.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Project_B/Maps/WeaponSpawnManager.h"
#include "Project_B/Maps/LobbyMap/BanimalsGameInstance.h"
#include "Project_B/Maps/LuggageChaos/Widget/LuggageScoreWidget.h"
#include "Project_B/Maps/Base/Widget/GameReadyWidget.h"
#include "Project_B/Maps/Base/Widget/GameEndWidget.h"
#include "Project_B/Maps/BlackHole/Public/TargetActor.h"
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
	InitPlayerInfo();
	
	if (HasAuthority())
	{
		FTimerHandle OnStartTimerHandle;
		GetWorld()->GetTimerManager().SetTimer(OnStartTimerHandle, this, &ALuggageChaosGameState::GameStart,ReadyTime,false);
		InitSpawnPoint();
	}

	APlayerController* pc = GetWorld()->GetFirstPlayerController();
	InitUI(pc);
}

void ALuggageChaosGameState::InitPlayerInfo()
{
	UBanimalsGameInstance* gi = Cast<UBanimalsGameInstance>(GetWorld()->GetGameInstance());
	
	if (isDummyPlayerInfo)
	{
		PlayersInfo = DummyPlayersInfo();
	}
	else
	{
		PlayersInfo= gi->GetPlayerInfo();
		
		FTimerHandle LambdaTimerHandle;
		GetWorld()->GetTimerManager().SetTimer(LambdaTimerHandle, [this]()
		{
			const FUniqueNetIdRepl& NetIdRepl = GetWorld()->GetFirstPlayerController()->GetPlayerState<APlayerState>()->GetUniqueId();
			
			if (NetIdRepl.IsValid())
			{
				TSharedPtr<const FUniqueNetId> NetId = NetIdRepl.GetUniqueNetId();
				MyKey = NetId->ToString();
				UE_LOG(LogTemp, Error, TEXT("나의 키: %s"), *MyKey);

				FPlayerInfo* Info = PlayersInfo.Find(MyKey);
				if (Info)
				{
					if (Info->Team == ETeamType::Blue)
					{
						UE_LOG(LogTemp, Error, TEXT("나의 팀: Blue"));
					}
					else
					{
						UE_LOG(LogTemp, Error, TEXT("나의 팀: Red"));
					}
				}
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("NetIdRepl is invalid"));
			}
		}, 0.8f, false);
	}
}

void ALuggageChaosGameState::InitPlayerLoc(APawn* pawn,FString key)
{
	if (isDummyPlayerInfo)
	{
		FPlayerInfo* Info = PlayersInfo.Find(FString::FromInt(dummyKey));
		dummyKey++;
		
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

void ALuggageChaosGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ALuggageChaosGameState, WinnerKeys);
	DOREPLIFETIME(ALuggageChaosGameState, WeaponSpawnManager);
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

	if (HasAuthority())
	{
		WeaponSpawnManager = Cast<AWeaponSpawnManager>(GetWorld()->SpawnActor(AWeaponSpawnManager::StaticClass()));
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

void ALuggageChaosGameState::AddWinner(FString playerKey)
{
	WinnerKeys.Add(playerKey);
}

void ALuggageChaosGameState::GameEnd()
{
	FTimerHandle OnEndTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(OnEndTimerHandle, this, &ALuggageChaosGameState::ChangeLevelPodium,EndTime*SlowTime,false);
	
	UBanimalsGameInstance* gi = Cast<UBanimalsGameInstance>(GetWorld()->GetGameInstance());
	gi->WinnerKeys = WinnerKeys;
	
	for (int i = 0; i<WinnerKeys.Num(); i++)
	{
		UE_LOG(LogTemp,Error,TEXT("gi에 승리자 키: %s 저장"), *WinnerKeys[i]);
	}
	
	Net_GameEnd();
}

void ALuggageChaosGameState::Net_GameEnd_Implementation()
{
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), SlowTime);
}

void ALuggageChaosGameState::ChangeLevelPodium()
{
	LOG_SCREEN("레벨 전환");
	GetWorld()->ServerTravel(TEXT("/Game/Maps/Podium/LV_Podium01?listen"));
	
}

void ALuggageChaosGameState::TimeOut()
{
	Net_JudgeWinner();
}

void ALuggageChaosGameState::Win(ETeamType winner)
{
	WinnerTeam = winner;
	
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
		pc->GetPlayerState<ALuggagePlayerState>()->Server_Win();
		AddWinPrize(pc);
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

void ALuggageChaosGameState::OnPlayerDeath(APlayerController* pc)
{
	Server_OnPlayerDeath(pc);
}

void ALuggageChaosGameState::Server_OnPlayerDeath_Implementation(APlayerController* pc)
{
	LOG_SCREEN("서버: 죽일게");
	
	AddDeadPlayer(pc);
	
	FTimerHandle respawnTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(respawnTimerHandle, this,&ALuggageChaosGameState::Respawn,RespawnTime, false);

	if (pc != GetWorld()->GetFirstPlayerController())
	{
		Net_OnPlayerDeath(pc);
		return;
	}

	LOG_SCREEN("tjqj: 죽을게");
	ABaseCharacter* player = Cast<ABaseCharacter>(pc->GetPawn());
	player->IsDead = true;
	
	pc->GetPawn()->DisableInput(pc);
	DeathEffects(pc);
	
	FTimerHandle deadTimerHandle;
	GetWorldTimerManager().SetTimer(deadTimerHandle, FTimerDelegate::CreateLambda([player, pc, this]()
	{
		LOG_SCREEN("관전자로 전환");
		player->SetActorHiddenInGame(true);
		player->SetActorEnableCollision(false);
		pc->StartSpectatingOnly();
	}), DeadTime, false);
}

void ALuggageChaosGameState::Net_OnPlayerDeath_Implementation(APlayerController* pc)
{
	LOG_SCREEN("클라: 죽을게");
	ABaseCharacter* player = Cast<ABaseCharacter>(pc->GetPawn());
	player->IsDead = true;
	
	pc->GetPawn()->DisableInput(pc);
	DeathEffects(pc);
	
	FTimerHandle deadTimerHandle;
	GetWorldTimerManager().SetTimer(deadTimerHandle, FTimerDelegate::CreateLambda([player, pc, this]()
	{
		LOG_SCREEN("관전자로 전환");
		player->SetActorHiddenInGame(true);
		player->SetActorEnableCollision(false);
		pc->StartSpectatingOnly();
	}), DeadTime, false);
}

void ALuggageChaosGameState::AddDeadPlayer(APlayerController* pc)
{
	DeadPawns.Enqueue(pc->GetPawn());
	DeadPlayers.Enqueue(pc->GetPlayerState<APlayerState>());
}

void ALuggageChaosGameState::DeathEffects(APlayerController* pc)
{
	ABaseCharacter* Player = Cast<ABaseCharacter>(pc->GetPawn());
	
	if (Player->CameraComp)
	{
		Player->CameraComp->PostProcessSettings.ColorSaturation = FVector4(0, 0, 0, 1);
	}
}

void ALuggageChaosGameState::Respawn()
{
	LOG_SCREEN("서버: 살릴게");
	APlayerState* ps;
	APawn* pawn;
	DeadPawns.Dequeue(pawn);
	DeadPlayers.Dequeue(ps);

	const FUniqueNetIdRepl& NetIdRepl = ps->GetUniqueId();
	FString key;
	
	TSharedPtr<const FUniqueNetId> NetId = NetIdRepl.GetUniqueNetId();
	key = NetId->ToString();

	if (PlayersInfo.Find(key) == nullptr)
	{
		pawn->SetActorLocation(BlueSpawnPoints[0]->GetActorLocation());
		pawn->SetActorRotation(BlueSpawnPoints[0]->GetActorRotation());
	}
	else if (PlayersInfo.Find(key)->Team == ETeamType::Blue)
	{
		pawn->SetActorLocation(BlueSpawnPoints[0]->GetActorLocation());
		pawn->SetActorRotation(BlueSpawnPoints[0]->GetActorRotation());
	}
	else
	{
		pawn->SetActorLocation(RedSpawnPoints[0]->GetActorLocation());
		pawn->SetActorRotation(RedSpawnPoints[0]->GetActorRotation());
	}

	if (key != MyKey)
	{
		Net_OnPlayerRespawn(ps->GetPlayerController(), pawn);
		return;
	}

	APlayerController* pc = ps->GetPlayerController();
	
	pc->GetPlayerState<APlayerState>()->SetIsSpectator(false);
	pc->GetPlayerState<APlayerState>()->SetIsOnlyASpectator(false);
	
	pc->Possess(pawn);
	ABaseCharacter* player = Cast<ABaseCharacter>(pawn);
	player->IsDead = true;
	
	pawn->EnableInput(pc);
	player->CameraComp->PostProcessSettings.ColorSaturation = FVector4(1, 1, 1, 1);
	
	player->SetActorHiddenInGame(false);
	player->SetActorEnableCollision(true);
}

void ALuggageChaosGameState::Net_OnPlayerRespawn_Implementation(APlayerController* pc, APawn* pawn)
{
	LOG_SCREEN("클라: 리스폰!");
	
	pc->GetPlayerState<APlayerState>()->SetIsSpectator(false);
	pc->GetPlayerState<APlayerState>()->SetIsOnlyASpectator(false);
	
	pc->Possess(pawn);
	ABaseCharacter* player = Cast<ABaseCharacter>(pawn);
	player->IsDead = true;
	
	pawn->EnableInput(pc);
	player->CameraComp->PostProcessSettings.ColorSaturation = FVector4(1, 1, 1, 1);
	
	player->SetActorHiddenInGame(false);
	player->SetActorEnableCollision(true);
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
