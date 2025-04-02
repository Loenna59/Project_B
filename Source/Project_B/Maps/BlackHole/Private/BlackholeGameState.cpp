// Fill out your copyright notice in the Description page of Project Settings.

// 게임의 실시간 상태 및 점수 관리
// 팀 점수 및 게임 진행 상태
// 게임 종료시 승리 팀 계산


#include "Project_B/Maps/BlackHole/Public/BlackholeGameState.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/PlayerState.h"
#include "GameFramework/SpectatorPawn.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Project_B/Maps/BlackHole/Public/BlackHole.h"
#include "Project_B/Maps/BlackHole/Public/BlackholePlayerState.h"
#include "Project_B/Maps/BlackHole/Public/BlackholeSpectator.h"
#include "Project_B/Maps/BlackHole/Public/DestroyZone.h"
#include "Project_B/Maps/BlackHole/Public/TargetActor.h"
#include "Project_B/Maps/LobbyMap/BanimalsGameInstance.h"

class ABlackholePlayerState;

ABlackholeGameState::ABlackholeGameState()
{
	bReplicates = true;
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

	// 게임 시간
	GameStartTime = GetWorld()->GetTimeSeconds();
	// 게임 시작 30초후 첫번째 블랙홀을 보이게 한다	
	GetWorld()->GetTimerManager().SetTimer(BlackholeSpawnHandle, this, &ABlackholeGameState::SpawnBlackhole, 5.0f, false);
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
	GetWorld()->GetTimerManager().SetTimer(BlackholeDestroyHandle, this, &ABlackholeGameState::DestroyBlackhole, 10.0f, false);
}

void ABlackholeGameState::DestroyBlackhole()
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

	AlivePlayers = 0; // 초기화
	// 플레이어 정보 순회 (처음부터 세기)
	for (auto& it : InfoMap)
	{
		FPlayerInfo& PlayerInfo = it.Value;
		// 살아있다면
		if (PlayerInfo.bIsAlive)
		{
			AlivePlayers++;
		}
	}

	// TODO: 종료 조건: "한 팀만 남았거나" 플레이어가 1명 이하일 때
	if (AlivePlayers <= 1)
	{
		DetermineWinner();
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
	DeathEffects(PlayerController);
	
	// 입력 비활성화하고
	PlayerController->SetIgnoreLookInput(true);
	PlayerController->SetIgnoreMoveInput(true);
    
	// 관전자 모드로 전환
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateUObject(this, &ABlackholeGameState::ConvertToSpectator, PlayerController), 3.0f, false);
}

void ABlackholeGameState::DetermineWinner()
{
	if (!HasAuthority()) return;
	
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

	// TODO: 슬로우모션 동기화
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.5);
	UE_LOG(LogTemp, Warning, TEXT("게임 종료! 승자가 결정되었습니다!"));
}

void ABlackholeGameState::OnPlayerDeath(APlayerController* PlayerController)
{
	if (!HasAuthority()) return; // 서버만
	
	// 서버에서 사망 플레이어를 추가한다
	AddDeadPlayer(PlayerController);

	// 사망 처리 로직
	Multicast_PlayerDeath(PlayerController);
	
	CheckGameEndConditions();
	UE_LOG(LogTemp, Warning, TEXT("OnPlayerDeath"));
}

void ABlackholeGameState::DeathEffects(APlayerController* PlayerController)
{
	ABaseCharacter* Player = Cast<ABaseCharacter>(PlayerController->GetPawn());
	
	// 카메라 흑백 효과
	if (Player->CameraComp)
	{
		Player->CameraComp->PostProcessSettings.ColorSaturation = FVector4(0, 0, 0, 1);
		UE_LOG(LogTemp, Warning, TEXT("카메라 흑백 전환합니다"));
	}
}

void ABlackholeGameState::ConvertToSpectator(APlayerController* PlayerController)
{
	APawn* ControlledPawn = PlayerController->GetPawn();
	
	ControlledPawn->SetActorHiddenInGame(true);
	ControlledPawn->SetActorEnableCollision(false);
	UE_LOG(LogTemp, Warning, TEXT("ConvertToSpectator"));

	PlayerController->UnPossess();

	if (PlayerController == GetWorld()->GetFirstPlayerController())
	{
		UE_LOG(LogTemp, Warning, TEXT("SamePlayerController!!!!!!! : %p || %p"), PlayerController, GetWorld()->GetFirstPlayerController());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Different Controller : %p || %p"), PlayerController, GetWorld()->GetFirstPlayerController());
	}
	
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