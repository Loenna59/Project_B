// Fill out your copyright notice in the Description page of Project Settings.

// 게임의 실시간 상태 및 점수 관리
// 팀 점수 및 게임 진행 상태
// 게임 종료시 승리 팀 계산


#include "Project_B/Maps/BlackHole/Public/BlackholeGameState.h"

#include "Blueprint/UserWidget.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/PlayerState.h"
#include "GameFramework/SpectatorPawn.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Project_B/Maps/Base/Widget/GameEndWidget.h"
#include "Project_B/Maps/Base/Widget/GameReadyWidget.h"
#include "Project_B/Maps/WeaponSpawnManager.h"
#include "Project_B/Maps/BlackHole/Public/BlackHole.h"
#include "Project_B/Maps/BlackHole/Public/BlackholePlayerState.h"
#include "Project_B/Maps/BlackHole/Public/BlackholeSpectator.h"
#include "Project_B/Maps/BlackHole/Public/DestroyZone.h"
#include "Project_B/Maps/BlackHole/Public/TargetActor.h"
#include "Project_B/Maps/LobbyMap/BanimalsGameInstance.h"
#include "Project_B/Maps/Podium/WinnerPrize.h"

class ABlackholePlayerState;

ABlackholeGameState::ABlackholeGameState()
{
	bReplicates = true;
	BlackholeSpawnCount = 0;
}


void ABlackholeGameState::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp,Warning,TEXT("ABlackholeGameState::BeginPlay"));
	// 게임 인스턴스
	gi = Cast<UBanimalsGameInstance>(GetWorld()->GetGameInstance());
	
	// 블랙홀 + 회전체
	Blackhole = Cast<ABlackHole>(UGameplayStatics::GetActorOfClass(GetWorld(), ABlackHole::StaticClass()));
	Rotator = Cast<ADestroyZone>(UGameplayStatics::GetActorOfClass(GetWorld(), ADestroyZone::StaticClass()));

	GameReady();
	WeaponSpawnManager = Cast<AWeaponSpawnManager>(GetWorld()->SpawnActor(AWeaponSpawnManager::StaticClass()));
}

void ABlackholeGameState::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABlackholeGameState::GetLifetimeReplicatedProps(
	TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABlackholeGameState, GameStartTime);
	DOREPLIFETIME(ABlackholeGameState, AlivePlayers);
	DOREPLIFETIME(ABlackholeGameState, DeadPlayers);
	DOREPLIFETIME(ABlackholeGameState, WinnerKeys);
}

void ABlackholeGameState::GameReady()
{
	InitPlayerInfo();
	
	if (HasAuthority())
	{
		FTimerHandle OnStartTimerHandle;
		GetWorld()->GetTimerManager().SetTimer(OnStartTimerHandle, this, &ABlackholeGameState::StartGame,10.f,false);
	}

	APlayerController* pc = GetWorld()->GetFirstPlayerController();
	InitUI(pc);
}

void ABlackholeGameState::InitUI(APlayerController* pc)
{
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

void ABlackholeGameState::StartGame()
{
	if (HasAuthority())
	{
		// 게임 시간
		GameStartTime = GetWorld()->GetTimeSeconds();
		// TODO: 게임 시작 30초후 첫번째 블랙홀을 보이게 한다	
		GetWorld()->GetTimerManager().SetTimer(BlackholeSpawnHandle, this, &ABlackholeGameState::SpawnBlackhole, 30.0f, false);
	}
    
	// 모든 플레이어를 살아있는 상태로 초기화
	PlayersInfo = gi->GetPlayerInfo();
	for (auto& it : PlayersInfo)
	{
		it.Value.bIsAlive = true;
		it.Value.bIsWin = false;
	}
	
	MulticastRPC_GameStart();
}

void ABlackholeGameState::InitPlayerInfo()
{
	PlayersInfo = gi->GetPlayerInfo();
	FTimerHandle LambdaTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(LambdaTimerHandle, [this]()
	{
		const FUniqueNetIdRepl& NetIdRepl = GetWorld()->GetFirstPlayerController()->GetPlayerState<APlayerState>()->GetUniqueId();
			
		if (NetIdRepl.IsValid())
		{
			TSharedPtr<const FUniqueNetId> NetId = NetIdRepl.GetUniqueNetId();
			MyKey = NetId->ToString();
			UE_LOG(LogTemp, Error, TEXT("나의 키: %s"), *MyKey);
		}
	},0.8f, false);
}


void ABlackholeGameState::MulticastRPC_GameStart_Implementation()
{
	if (GameReadyWidget)
	{
		GameReadyWidget->PlayAnimLoadComplete();
	}
}

void ABlackholeGameState::SpawnBlackhole()
{
	if (!HasAuthority()) return; // 서버만 실행
	
	// 블랙홀 스폰 함수
	Blackhole->bIsActive = true;
	Rotator->Rotate(true);
	// 4페이즈까지만 있다
	if(BlackholeSpawnCount >=4) return;

	// 10초 후 블랙홀 소멸
	GetWorld()->GetTimerManager().SetTimer(BlackholeDestroyHandle, this, &ABlackholeGameState::DestroyBlackhole, 15.0f, false);
}

void ABlackholeGameState::DestroyBlackhole()
{
	if (!HasAuthority()) return;
	
	Blackhole->bIsActive = false;
	BlackholeSpawnCount++;
	Rotator->Rotate(false);

	// 재소환 예약
	if (BlackholeSpawnCount < 4)
	{
		GetWorld()->GetTimerManager().SetTimer(BlackholeSpawnHandle, this, &ABlackholeGameState::SpawnBlackhole, 30.0f, false);
	}
}

void ABlackholeGameState::CheckGameEndConditions()
{
	if (!HasAuthority()) return;
	
	// 게임 인스턴스에서 플레이어 정보 확인
	PlayersInfo = gi->GetPlayerInfo();

	AlivePlayers = 0; // 초기화
	
	// 플레이어 정보 순회 (처음부터 세기)
	// 생존 플레이어 수 계산 및 팀별 생존자 수 집계
	TMap<ETeamType, int32> TeamAliveCounts;
	TArray<ETeamType> RemainingTeams;
    
	for (auto& it : PlayersInfo)
	{
		FPlayerInfo& PlayerInfo = it.Value;
		if (PlayerInfo.bIsAlive)
		{
			AlivePlayers++;
			TeamAliveCounts.FindOrAdd(PlayerInfo.Team)++;
            
			if (!RemainingTeams.Contains(PlayerInfo.Team))
			{
				RemainingTeams.Add(PlayerInfo.Team);
			}
		}
	}

	// 1. 단일 팀만 남은 경우 (2명 이상이 같은 팀일 때)
	if (RemainingTeams.Num() == 1)
	{
		DetermineTeamWinner(RemainingTeams[0]);
		UE_LOG(LogTemp, Warning, TEXT("단일 팀만 남았습니다"));
		UE_LOG(LogTemp, Warning, TEXT("%d"), RemainingTeams[0]);
		return;
	}
    
	// 2. 1명만 남은 경우
	if (AlivePlayers == 1)
	{
		for (auto& it : PlayersInfo)
		{
			FPlayerInfo& PlayerInfo = it.Value;
			if (PlayerInfo.bIsAlive && PlayerInfo.Team != ETeamType::None)
			{
				DetermineTeamWinner(PlayerInfo.Team);
				UE_LOG(LogTemp, Warning, TEXT("플레이어 한명만 남았음, 승자결정합니다"));
				UE_LOG(LogTemp, Warning, TEXT("%d"), PlayerInfo.Team);
				return;
			}
		}
	}
}

// 플레이어가 죽으면 사망한 플레이어 목록을 추가하자
void ABlackholeGameState::AddDeadPlayer(APlayerController* PlayerController)
{
	if (HasAuthority()) // 서버
	{
		DeadPlayers.AddUnique(PlayerController);
	}
}

void ABlackholeGameState::Multicast_PlayerDeath_Implementation(APlayerController* PlayerController)
{
	if (!PlayerController) return;
	UE_LOG(LogTemp, Warning, TEXT("Multicast_PlayerDeath_Implementation"));
	
	// 죽은 플레이어에게 사망 효과 적용
	PlayerController->DisableInput(PlayerController);
	DeathEffects(PlayerController);
    
	// 관전자 모드로 전환
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateUObject(this, &ABlackholeGameState::ConvertToSpectator, PlayerController), 3.0f, false);
}

void ABlackholeGameState::GameEnd()
{
	FTimerHandle OnEndTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(OnEndTimerHandle, this, &ABlackholeGameState::ChangeLevelPodium,3.f,false);
	UE_LOG(LogTemp, Warning, TEXT("게임끝!!!!!"));
	
	gi->WinnerKeys = WinnerKeys;
	
	for (int i = 0; i<WinnerKeys.Num(); i++)
	{
		UE_LOG(LogTemp,Error,TEXT("gi에 승리자 키: %s 저장"), *WinnerKeys[i]);
	}
	
	MulticastRPC_GameEnd();
}

void ABlackholeGameState::MulticastRPC_GameEnd_Implementation()
{
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.5);
}

void ABlackholeGameState::ChangeLevelPodium()
{
	GetWorld()->ServerTravel(TEXT("/Game/Maps/Podium/LV_Podium01?listen"));
}

void ABlackholeGameState::DetermineTeamWinner(ETeamType WinningTeam)
{
	if (!HasAuthority()) return;
	WinnerTeam = WinningTeam;

	UE_LOG(LogTemp,Warning,TEXT("승리자 결정"));
	
	FPlayerInfo* myInfo = PlayersInfo.Find(MyKey);
	
	if (myInfo->Team == WinningTeam)
	{
		GameEndWidget->ShowVictory();
		
		APlayerController* pc = GetWorld()->GetFirstPlayerController();
		pc->GetPlayerState<ABlackholePlayerState>()->Server_Win();
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

void ABlackholeGameState::AddWinner(FString playerKey)
{
	WinnerKeys.Add(playerKey);
}

void ABlackholeGameState::AddWinPrize(APlayerController* pc)
{
	AWinnerPrize* prize = GetWorld()->SpawnActor<AWinnerPrize>(WinnerPrizeClass, pc->GetCharacter()->GetTransform());
	if (prize)
	{
		prize->AttachToComponent(pc->GetCharacter()->GetMesh(), 
								 FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	}
}

void ABlackholeGameState::OnPlayerDeath(APlayerController* PlayerController)
{
	ServerRPC_PlayerDeath(PlayerController);
}

void ABlackholeGameState::DeathEffects(APlayerController* PlayerController)
{
	ABaseCharacter* Player = Cast<ABaseCharacter>(PlayerController->GetPawn());
	Player->SetDie();
	
	// 카메라 흑백 효과
	if (Player->CameraComp)
	{
		Player->CameraComp->PostProcessSettings.ColorSaturation = FVector4(0, 0, 0, 1);
		UE_LOG(LogTemp, Warning, TEXT("카메라 흑백 전환합니다"));
	}
}

void ABlackholeGameState::ServerRPC_PlayerDeath_Implementation(APlayerController* PlayerController)
{
	// 서버에서 사망 플레이어를 추가한다
	AddDeadPlayer(PlayerController);

	// 사망 처리 로직
	Multicast_PlayerDeath(PlayerController);

	// 사망자 수 세기
	CheckGameEndConditions();
}

void ABlackholeGameState::ConvertToSpectator(APlayerController* PlayerController)
{
	APawn* ControlledPawn = PlayerController->GetPawn();
	
	ControlledPawn->SetActorHiddenInGame(true);
	ControlledPawn->SetActorEnableCollision(false);
	UE_LOG(LogTemp, Warning, TEXT("ConvertToSpectator"));

	PlayerController->UnPossess();

	AActor* Target = UGameplayStatics::GetActorOfClass(GetWorld(), ATargetActor::StaticClass());
	if (Target)
	{
		// SpectatorPawn 생성 및 전환
		ABlackholeSpectator* Spectator = GetWorld()->SpawnActorDeferred<ABlackholeSpectator>(SpectatorPawnClass, Target->GetActorTransform(), PlayerController, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

		if (Spectator)
		{
			UGameplayStatics::FinishSpawningActor(Spectator, Target->GetActorTransform());
			PlayerController->Possess(Spectator);
			Spectator->CreateSpectatorUI();
			// 커서 보이게 하자
			GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(true);
			UE_LOG(LogTemp, Warning, TEXT("Spectator Possessed"));
		}
	}
}
