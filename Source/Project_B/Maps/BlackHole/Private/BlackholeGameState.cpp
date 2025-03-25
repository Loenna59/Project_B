// Fill out your copyright notice in the Description page of Project Settings.

// 게임의 실시간 상태 및 점수 관리
// 팀 점수 및 게임 진행 상태
// 게임 종료시 승리 팀 계산


#include "Project_B/Maps/BlackHole/Public/BlackholeGameState.h"

#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Project_B/Maps/BlackHole/Public/BlackHole.h"
#include "Project_B/Maps/BlackHole/Public/DestroyZone.h"

void ABlackholeGameState::BeginPlay()
{
	Super::BeginPlay();
	
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

	DOREPLIFETIME(ABlackholeGameState, );
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

void ABlackholeGameState::SetGameOver()
{
}
