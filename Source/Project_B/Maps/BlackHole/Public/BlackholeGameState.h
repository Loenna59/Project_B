// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/BaseCharacter.h"
#include "GameFramework/GameStateBase.h"
#include "BlackholeGameState.generated.h"

struct FPlayerInfo;
class UBanimalsGameInstance;
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


	// 게임 인스턴스
	UBanimalsGameInstance* gi;
	
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
	
	// 게임 승리/패배 결정을 위한 생존 플레이어 수 체크
	UPROPERTY(Replicated, VisibleAnywhere)
	int32 AlivePlayers = 0;
	// 게임 종료 조건 확인
	UFUNCTION()
	void CheckGameEndConditions();

	// Replicated 변수 추가
	UPROPERTY(ReplicatedUsing = OnRep_PlayerDeathStates)
	TArray<APlayerController*> DeadPlayers;
	UFUNCTION()
	void OnRep_PlayerDeathStates();
	void AddDeadPlayer(APlayerController* PlayerController);
	
	// 승자 결정
	void DetermineWinner();
	// 죽음 처리
	void OnPlayerDeath(APlayerController* PlayerController);
	void DeathEffects(APlayerController* PlayerController);

	// 게임 시작
	UPROPERTY(Replicated)
	float GameStartTime = 0.0f;
	// UFUNCTION(NetMulticast, Reliable)
	// void MulticastRPC_SetGameStart(float StartTime);
	
	// 게임 종료
	// UFUNCTION(NetMulticast, Reliable)
	// void MulticastRPC_SetGameOver();

	UPROPERTY(Replicated)
	class AWeaponSpawnManager* WeaponSpawnManager;
};
