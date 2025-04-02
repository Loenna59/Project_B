#include "LuggageChaosGameState.h"

#include "LuggageChaosGameMode.h"
#include "LuggagePlayerState.h"

#include "Blueprint/UserWidget.h"
#include "Camera/CameraComponent.h"
#include "Character/BaseCharacter.h"
#include "Components/AudioComponent.h"
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
			
			ABaseCharacter* character = Cast<ABaseCharacter>(pawn);
			Net_InitCharacterSkin(character, ETeamType::Blue);
		}
		else
		{
			LOG_PRINT(TEXT("저는 레드팀"));
			pawn->SetActorLocation(RedSpawnPoints[redIdx]->GetActorLocation());
			pawn->SetActorRotation(RedSpawnPoints[blueIdx]->GetActorRotation());
			++redIdx;
			
			ABaseCharacter* character = Cast<ABaseCharacter>(pawn);
			Net_InitCharacterSkin(character, ETeamType::Red);
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
			
			ABaseCharacter* character = Cast<ABaseCharacter>(pawn);
			Net_InitCharacterSkin(character, ETeamType::Blue);
		}
		else
		{
			LOG_PRINT(TEXT("저는 레드팀"));
			pawn->SetActorLocation(RedSpawnPoints[redIdx]->GetActorLocation());
			pawn->SetActorRotation(RedSpawnPoints[blueIdx]->GetActorRotation());
			++redIdx;
			
			ABaseCharacter* character = Cast<ABaseCharacter>(pawn);
			Net_InitCharacterSkin(character, ETeamType::Red);
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


void ALuggageChaosGameState::Net_InitCharacterSkin_Implementation(ABaseCharacter* character, ETeamType team)
{
	if (team == ETeamType::Blue)
	{
		character->SetSkin(CharacterColor::Blue);
	}
	else
	{
		character->SetSkin(CharacterColor::Red);
	}
}

void ALuggageChaosGameState::GameStart()
{
	//TODO: 레벨 스트리밍 테스트 
	// ULevelStreaming* curLevel = GetWorld()->GetStreamingLevels()[0];
	// ULevelStreaming* nextLevel = GetWorld()->GetStreamingLevels()[1];
	// if (nextLevel && curLevel)
	// {
	// 	nextLevel->SetShouldBeVisible(true);
	// 	curLevel->SetShouldBeVisible(false);
	// }
	// else
	// {
	// 	LOG_ERROR(this,TEXT("레벨 없음"));
	// }
	
	Net_GameStart();
	
	GetWorld()->GetTimerManager().SetTimer(GameTimerHandle, this, &ALuggageChaosGameState::TimeOut,GameTime,false);
}

void ALuggageChaosGameState::Net_GameStart_Implementation()
{
	if (GameReadyWidget)
	{
		GameReadyWidget->PlayAnimLoadComplete();
	}
	if (BGM)
	{
		BgmComponent = UGameplayStatics::SpawnSound2D(GetWorld(), BGM);
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
	BgmComponent->Stop();
	if (SW_Win)
	{
		BgmComponent = UGameplayStatics::SpawnSound2D(GetWorld(), SW_Win);
	}
}

void ALuggageChaosGameState::ChangeLevelPodium()
{
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
		
		APlayerController* pc = GetWorld()->GetFirstPlayerController();
		pc->GetPlayerState<ALuggagePlayerState>()->Server_Win();
		AddWinPrize(pc);
	}
	else
	{
		GameEndWidget->ShowDefeat();
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

	ABaseCharacter* Character = Cast<ABaseCharacter>(pc->GetCharacter());
	if (Character)
	{
		Character->SetSunglasses(true);
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
	AddDeadPlayer(pc);
	
	ABaseCharacter* player = Cast<ABaseCharacter>(pc->GetPawn());
	
	// 흑백으로 죽은 자기 자신 보는 상태
	Net_OnPlayerDeath(player, pc);

	// 관전모드 진입
	FTimerHandle deadTimerHandle;
	GetWorldTimerManager().SetTimer(deadTimerHandle, FTimerDelegate::CreateLambda([player, pc, this]()
	{
		Net_OnPlayerSpectate(player,pc);
	}), DeadTime, false);

	// 리스폰
	FTimerHandle respawnTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(respawnTimerHandle, this,&ALuggageChaosGameState::Respawn,RespawnTime, false);
}

// 흑백으로 죽은 자기 자신 보는 상태
void ALuggageChaosGameState::Net_OnPlayerDeath_Implementation(ABaseCharacter* player, APlayerController* pc)
{
	// 모든 클라이언트
	player->SetDie();
	
	if (pc == nullptr)
	{
		return;
	}

	if (pc->IsLocalController())
	{
		pc->GetPawn()->DisableInput(pc);
		DeathEffects(pc);
	}
}

// 관전모드 진입
void ALuggageChaosGameState::Net_OnPlayerSpectate_Implementation(ABaseCharacter* player, APlayerController* pc)
{
	// 모든 클라이언트
	player->SetActorHiddenInGame(true);
	player->SetActorEnableCollision(false);

	if (pc == nullptr)
	{
		return;
	}
	
	pc->StartSpectatingOnly();
}

void ALuggageChaosGameState::Respawn()
{
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

	ps->SetIsSpectator(false);
	ps->SetIsOnlyASpectator(false);
	
	APlayerController* pc = ps->GetPlayerController();
	
	pc->Possess(pawn);

	ABaseCharacter* player = Cast<ABaseCharacter>(pc->GetPawn());
	
	Net_OnPlayerRespawn(pc, player);
}

void ALuggageChaosGameState::Net_OnPlayerRespawn_Implementation(APlayerController* pc, ABaseCharacter* player)
{
	player->SetActorHiddenInGame(false);
	player->SetActorEnableCollision(true);
	player->Rebirth();
	
	if (pc == nullptr)
	{
		return;
	}
	
	
	if (pc->IsLocalController())
	{
		FInputModeGameOnly InputMode;
		pc->SetInputMode(InputMode);
	
		pc->GetPawn()->EnableInput(pc);
	
		player->CameraComp->PostProcessSettings.ColorSaturation = FVector4(1, 1, 1, 1);
	}
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
