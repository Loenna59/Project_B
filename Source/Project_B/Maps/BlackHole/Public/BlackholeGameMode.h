// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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
	
	// 게임 진행 시간 (3분)
	
	// 현재 게임시간
	float CurrentGameTime = 0.0f;
	void SpawnBlackhole();

	// 블랙홀 소환 타이머 (게임시작->30초후, 페이즈완전히 종료 30초후)
	FTimerHandle BlackholeSpawnTimerHandle;

	// 블랙홀 소환 횟수
	int32 BlackholeSpawnCount = 0;
};
