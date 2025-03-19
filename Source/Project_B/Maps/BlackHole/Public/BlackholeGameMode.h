// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BlackHole.h"
#include "GameFramework/GameModeBase.h"
#include "BlackholeGameMode.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_B_API ABlackholeGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ABlackholeGameMode();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	// 게임 종료 함수
	void EndGame();
	
	// 게임 진행 타이머 (3분)
	FTimerHandle GameTimerHandle;
	
	// 블랙홀
	ABlackHole* Blackhole = nullptr;
	void SpawnBlackhole();
	void DestroyBalckhole();
	
	// 블랙홀 타이머 (게임시작->30초후, 페이즈완전히 종료 30초후)
	FTimerHandle BlackholeSpawnHandle;
	FTimerHandle BlackholeDestroyHandle;

	// 블랙홀 소환 횟수
	int32 BlackholeSpawnCount = 0;
};
