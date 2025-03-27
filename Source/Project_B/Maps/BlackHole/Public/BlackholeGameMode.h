// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Project_B/Project_BGameMode.h"
#include "BlackholeGameMode.generated.h"

class ABlackholeGameState;
/**
 * 
 */
UCLASS()
class PROJECT_B_API ABlackholeGameMode : public AProject_BGameMode
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;
	// 서버 게임 세션에 접속하면 자동 호출
	virtual void PostLogin(APlayerController* NewPlayer) override;
	
public:
	int32 playerIdx = 0;
	ABlackholeGameState* gs;

	// 게임 제한 시간 (3분)
	// 게임 종료 함수
	FTimerHandle timerHandle;
	void EndGame();

	/*
	// 플레이어 사망처리
	UFUNCTION(BlueprintCallable)
	void OnPlayerDeath(APlayerController* PlayerController);*/
	
	// 게임 종료 로직
	void CheckGameEndConditions();
};
