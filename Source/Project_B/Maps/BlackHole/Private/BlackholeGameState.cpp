// Fill out your copyright notice in the Description page of Project Settings.

// 게임의 실시간 상태 및 점수 관리
// 팀 점수 및 게임 진행 상태
// 게임 종료시 승리 팀 계산


#include "Project_B/Maps/BlackHole/Public/BlackholeGameState.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Project_B/Maps/BlackHole/Public/BlackHole.h"
#include "Project_B/Maps/BlackHole/Public/DestroyZone.h"
#include "Project_B/Maps/BlackHole/Public/TargetActor.h"
#include "Project_B/Maps/LobbyMap/BanimalsGameInstance.h"

void ABlackholeGameState::BeginPlay()
{
	Super::BeginPlay();

	bReplicates = true;

	// 게임 인스턴스
	gi = Cast<UBanimalsGameInstance>(GetWorld()->GetGameInstance());
	
	// 블랙홀 + 회전체
	Blackhole = Cast<ABlackHole>(UGameplayStatics::GetActorOfClass(GetWorld(), ABlackHole::StaticClass()));
	Rotator = Cast<ADestroyZone>(UGameplayStatics::GetActorOfClass(GetWorld(), ADestroyZone::StaticClass()));

	// 게임 시작 30초후 첫번째 블랙홀을 보이게 한다
	// TODO: 실제 시연때는 30초로 변경하기
	GetWorld()->GetTimerManager().SetTimer(BlackholeSpawnHandle, this, &ABlackholeGameState::SpawnBlackhole, 30.0f, false);
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
}


void ABlackholeGameState::SpawnBlackhole()
{
	// 블랙홀 스폰 함수
	Blackhole->bIsActive = true;
	Rotator->Rotate(true);
	// 4페이즈까지만 있다
	if(BlackholeSpawnCount >=4) return;

	// 10초 후 블랙홀 소멸
	GetWorld()->GetTimerManager().SetTimer(BlackholeDestroyHandle, this, &ABlackholeGameState::DestroyBalckhole, 10.0f, false);
}

void ABlackholeGameState::DestroyBalckhole()
{
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
	// 게임 인스턴스에서 플레이어 정보 확인
	TMap<FString, FPlayerInfo>& InfoMap = gi->GetPlayerInfo();

	AlivePlayers = 0;
	// 플레이어 정보 순회
	for (auto& it : InfoMap)
	{
		FPlayerInfo& PlayerInfo = it.Value;
		// 살아있다면
		if (PlayerInfo.bIsAlive)
		{
			AlivePlayers++;
		}
	}

	// TODO: 종료 조건: 한 팀만 남았거나 플레이어가 1명 이하일 때
	if (AlivePlayers <= 1)
	{
		DetermineWinner();
	}
}


void ABlackholeGameState::AddDeadPlayer(APlayerController* PlayerController)
{
	if (HasAuthority())
	{
		DeadPlayers.AddUnique(PlayerController);
		OnRep_PlayerDeathStates(); // 서버에서 즉시 실행
	}
}

void ABlackholeGameState::OnRep_PlayerDeathStates()
{
	for (APlayerController* PC : DeadPlayers)
	{
		if (PC && PC->IsLocalController()) // 로컬 플레이어만 처리
		{
			DeathEffects(PC);
		}
	}
}

void ABlackholeGameState::DetermineWinner()
{
	for (APlayerState* PS : PlayerArray)
	{
		if (PS && !DeadPlayers.Contains(PS->GetPlayerController()))
		{
			if (gi)
			{
				gi->SetPlayerWinInfo(PS->GetUniqueId()->ToString(), true);
			}
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("Game Over! Winner determined."));
}


void ABlackholeGameState::MulticastRPC_SetGameStart_Implementation(float StartTime)
{
	// 게임 시작 로직
	GameStartTime = StartTime;
}

void ABlackholeGameState::MulticastRPC_SetGameOver_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("End Game!!!!"));
	
	// 승패 가르기
	DetermineWinner();
}

void ABlackholeGameState::OnPlayerDeath(APlayerController* PlayerController)
{
	if (!HasAuthority()) return;
	
	CheckGameEndConditions();
	// 서버에서 사망 플레이어를 추가한다
	AddDeadPlayer(PlayerController);

	// 사망 처리 로직
	ABaseCharacter* Player = Cast<ABaseCharacter>(PlayerController->GetPawn());
	UE_LOG(LogTemp, Warning, TEXT("Player Death"));

	// 사망 처리 로직
	PlayerController->SetIgnoreLookInput(true);
	PlayerController->SetIgnoreMoveInput(true);
	
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([Player, PlayerController, this]()
	{
	   AActor* Target = UGameplayStatics::GetActorOfClass(GetWorld(), ATargetActor::StaticClass());
	   if (Target)
	   {
		  PlayerController->SetViewTarget(Target);
		  Player->SetActorHiddenInGame(true);
		  Player->SetActorEnableCollision(false);
	   }
	}), 3.0f, false);
}

void ABlackholeGameState::DeathEffects(APlayerController* PlayerController)
{
	ABaseCharacter* Player = Cast<ABaseCharacter>(PlayerController->GetPawn());

	UE_LOG(LogTemp, Warning, TEXT("카메라 흑백효과입니다"));
	// 카메라 흑백 효과
	if (Player->CameraComp)
	{
		Player->CameraComp->PostProcessSettings.ColorSaturation = FVector4(0, 0, 0, 1);
		UE_LOG(LogTemp, Warning, TEXT("카메라 전환합니다"));
	}
}





