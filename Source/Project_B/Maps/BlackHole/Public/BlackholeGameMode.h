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
	UPROPERTY(EditAnywhere, Category="BlackholeSettings")
	float GameDuration = 180.f;
	// 블랙홀 생성 시간 (주기)
	// 블랙홀 가져오기
	// 블랙홀 소환위치값? (0,0,700)
	UPROPERTY(EditAnywhere, Category="BlackholeSettings")
	FVector BlackholeSpawnPos = FVector(0,0,700);

	// 현재 게임시간
	float CurrentGameTime =0.0f;
	void SpawnBlackhole();
	
};

inline void ABlackholeGameMode::SpawnBlackhole()
{
	// 블랙홀 스폰 함수
}
