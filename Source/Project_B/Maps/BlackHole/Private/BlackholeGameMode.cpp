// Fill out your copyright notice in the Description page of Project Settings.

// 블랙홀 맵 게임 전반적인 규칙관리
// 게임 시작 및 초기 설정 (인원제한, 인원배치위치, 팀 자동배정)
// 게임 진행 관리 (제한시간 설정, 종료 조건확인)
// 승리/패배 팀 결정 및 최종 결과 전달

#include "Project_B/Maps/BlackHole/Public/BlackholeGameMode.h"

#include "Kismet/GameplayStatics.h"
#include "Project_B/Maps/BlackHole/Public/BlackholeGameState.h"
#include "Project_B/Maps/BlackHole/Public/BlackholePlayerState.h"
#include "Project_B/Maps/BlackHole/Public/TargetActor.h"
#include "Project_B/Maps/LobbyMap/BanimalsGameInstance.h"

void ABlackholeGameMode::BeginPlay()
{
	Super::BeginPlay();
	
	float StartTime = GetWorld()->GetTimeSeconds();
	// 3분후에는 게임 종료 함수가 호출된다
	GetWorld()->GetTimerManager().SetTimer(timerHandle, this, &ABlackholeGameMode::EndGame, 180.f, false);	

	// 게임 시작 알리기
	gs = GetGameState<ABlackholeGameState>();
	if (gs)
	{
		gs->MulticastRPC_SetGameStart(StartTime);
	}
}

void ABlackholeGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	
	// Legacy: 8명 넘으면 함수 나가자
	int32 MaxPlayers = 8;
	if (playerIdx > MaxPlayers) return;

	// 플레이어와 스테이트 가져오자
	AActor* player = Cast<AActor>(NewPlayer->GetPawn());
	ABlackholePlayerState* ps = NewPlayer->GetPlayerState<ABlackholePlayerState>();
	
	// 원 중심 기준 플레이어 배치하기
	FVector CenterPos = FVector(0,0,450);
	float Radius = 800.f;
	float Angle = (2 * PI / MaxPlayers) * playerIdx;
	FVector NewPos = CenterPos + FVector(FMath::Cos(Angle), FMath::Sin(Angle), 0.0f) * Radius;
	player->SetActorLocation(NewPos);

	playerIdx++;
}

void ABlackholeGameMode::EndGame()
{
	UE_LOG(LogTemp, Warning, TEXT("End Game!!!!"));
	
	// 게임이 종료되면, 게임 스테이트에 전달하자
	if (gs)
	{
		gs->MulticastRPC_SetGameOver();
	}
}

void ABlackholeGameMode::OnPlayerDeath(APlayerController* PlayerController)
{
	// 플레이어 키값 가져올것
	FString Key;
	const FUniqueNetIdRepl& NetIdRepl = PlayerController->GetPlayerState<APlayerState>()->GetUniqueId();
	if (NetIdRepl.IsValid())
	{
		TSharedPtr<const FUniqueNetId> NetId = NetIdRepl.GetUniqueNetId();
		Key = NetId->ToString();
	}

	// 이 함수가 실행되면, 플레이어는 죽음 상태라고 하자
	UBanimalsGameInstance* gi = Cast<UBanimalsGameInstance>(GetWorld()->GetGameInstance());
	TMap<FString, FPlayerInfo> Info = gi->GetPlayerInfo();
	Info[Key].bIsAlive = false;

	// 플레이어의 화면을 흑백으로 전환하자

	
	/*
	// 3초뒤에 플레이어를 관전자 모드로 전환하자 (이때는 당연히 흑백이 아니어야함)
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([=]()
	{
		AActor* Target = UGameplayStatics::GetActorOfClass(GetWorld(), ATargetActor::StaticClass());
		if (Target)
		{
			PlayerController->SetViewTargetWithBlend(Target, 1.0f);
			UE_LOG(LogTemp, Log, TEXT("OnPlayerDeath: Player %s switched to spectator mode"), *Key);
		}
	}), 3.0f, false);
	*/
	
	// 이때, 조작은 클릭만 받게 해야함
	PlayerController->SetIgnoreMoveInput(true);
	PlayerController->SetIgnoreLookInput(true);
}
