// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "BlackholeGameState.generated.h"

class ABlackHole;
class ADestroyZone;
/**
 * 
 */
UCLASS()
class PROJECT_B_API ABlackholeGameState : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// 블랙홀 관련 내용
	// 회전체
	ADestroyZone* Rotator = nullptr;
	// 블랙홀
	ABlackHole* Blackhole = nullptr;
	void SpawnBlackhole();
	void DestroyBalckhole();
	// 블랙홀 타이머 (게임시작->30초후, 페이즈완전히 종료 30초후)
	FTimerHandle BlackholeSpawnHandle;
	FTimerHandle BlackholeDestroyHandle;
	// 블랙홀 소환 횟수
	int32 BlackholeSpawnCount = 0;
	
	// 게임 승리/패배 결정을 위한 스코어 처리

	// 게임 종료
	void SetGameOver();
};
