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
#include "Project_B/Utilities/LogMacro.h"

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

void ABlackholeGameState::DetermineWinner()
{
	TMap<FString, FPlayerInfo>& InfoMap = gi->GetPlayerInfo();
	for (auto& it : InfoMap)
	{
		FPlayerInfo& PlayerInfo = it.Value;
		PlayerInfo.bIsWin = PlayerInfo.bIsAlive;
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
	ABaseCharacter* Player = Cast<ABaseCharacter>(PlayerController->GetPawn());
    
	UE_LOG(LogTemp, Warning, TEXT("Player Death"));

	// 클라이언트에 RPC 호출
	ClinetRPC_OnPlayerDeath(Player);

	// 서버 측 사망 처리 로직
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

void ABlackholeGameState::ClinetRPC_OnPlayerDeath_Implementation(ABaseCharacter* Player)
{
	// 해당 플레이어의 로컬 플레이어 컨트롤러인 경우에만 실행
	APlayerController* LocalPlayerController = GetWorld()->GetFirstPlayerController();
	if (LocalPlayerController && LocalPlayerController->GetPawn() == Player)
	{
		// 카메라 흑백 처리
		if (Player->CameraComp)
		{
			Player->CameraComp->PostProcessSettings.ColorSaturation = FVector4(0, 0, 0, 1);
		}

		// 입력 차단
		LocalPlayerController->SetIgnoreLookInput(true);
		LocalPlayerController->SetIgnoreMoveInput(true);
	}

	// 플레이어 키값
	FString Key;
	const FUniqueNetIdRepl& NetIdRepl = GetWorld()->GetFirstPlayerController()->GetPlayerState<APlayerState>()->GetUniqueId();
	if (NetIdRepl.IsValid())
	{
		TSharedPtr<const FUniqueNetId> NetId = NetIdRepl.GetUniqueNetId();
		Key = NetId->ToString();
		UE_LOG(LogTemp, Warning, TEXT("blackhole: %s"), *Key);
	}
	TMap<FString, FPlayerInfo> Info = gi->GetPlayerInfo();
	FPlayerInfo* myInfo = Info.Find(Key);
	
	if (myInfo == nullptr)
	{
		LOG_ERROR(this,TEXT("나의 키: %s, 키가 없어욤"),*Key);
		return;
	}
}




