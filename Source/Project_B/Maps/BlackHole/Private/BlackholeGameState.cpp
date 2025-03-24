// Fill out your copyright notice in the Description page of Project Settings.


#include "Project_B/Maps/BlackHole/Public/BlackholeGameState.h"

#include "SAdvancedRotationInputBox.h"
#include "Kismet/GameplayStatics.h"
#include "Project_B/Maps/BlackHole/Public/BlackHole.h"
#include "Project_B/Maps/BlackHole/Public/BlackholeGameMode.h"
#include "Project_B/Maps/BlackHole/Public/DestroyZone.h"

void ABlackholeGameState::BeginPlay()
{
	Super::BeginPlay();

	// 블랙홀 + 회전체
	Blackhole = Cast<ABlackHole>(UGameplayStatics::GetActorOfClass(GetWorld(), ABlackHole::StaticClass()));
	Rotator = Cast<ADestroyZone>(UGameplayStatics::GetActorOfClass(GetWorld(), ADestroyZone::StaticClass()));
	// 게임 시작 30초후 첫번째 블랙홀을 보이게 한다
	// TODO: 실제 시연때는 30초로 변경하기
	GetWorld()->GetTimerManager().SetTimer(BlackholeSpawnHandle, this, &ABlackholeGameState::SpawnBlackhole, 5.0f, false);
}

void ABlackholeGameState::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
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
		GetWorld()->GetTimerManager().SetTimer(BlackholeSpawnHandle, this, &ABlackholeGameState::SpawnBlackhole, 5.0f, false);
	}
}
