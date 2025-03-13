// Fill out your copyright notice in the Description page of Project Settings.


#include "Project_B/Maps/BlackHole/Public/BlackholeGameMode.h"


ABlackholeGameMode::ABlackholeGameMode()
{
	PrimaryActorTick.bCanEverTick = true;
	
}

void ABlackholeGameMode::BeginPlay()
{
	Super::BeginPlay();

	// 게임 종료 타이머 설정

	// 블랙홀 생성 타이머 설정
}

void ABlackholeGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CurrentGameTime += DeltaTime;
}


inline void ABlackholeGameMode::SpawnBlackhole()
{
	// 블랙홀 스폰 함수
}

