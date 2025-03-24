// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BlackHole.h"
#include "DestroyZone.h"
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
};
