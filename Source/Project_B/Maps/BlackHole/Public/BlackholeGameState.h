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
	ABlackholeGameState();
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
	void DestroyBlackhole();
	// 블랙홀 타이머 (게임시작->30초후, 페이즈완전히 종료 30초후)
	FTimerHandle BlackholeSpawnHandle;
	FTimerHandle BlackholeDestroyHandle;
	// 블랙홀 소환 횟수
	int32 BlackholeSpawnCount = 0;
	
	// 게임 승리/패배 결정을 위한 생존 플레이어 수 체크
	UPROPERTY(Replicated, VisibleAnywhere)
	int32 AlivePlayers = 0;

	// Replicated 변수 추가
	// UPROPERTY(ReplicatedUsing = OnRep_PlayerDeathStates)
	UPROPERTY(Replicated = OnRep_PlayerDeathStates)
	TArray<APlayerController*> DeadPlayers;
	void AddDeadPlayer(APlayerController* PlayerController);

	// 게임 종료 조건 확인
	UFUNCTION()
	void CheckGameEndConditions();
	// 승자 결정
	void DetermineWinner();
	// 죽음 처리
	void OnPlayerDeath(APlayerController* PlayerController);
	void DeathEffects(APlayerController* PlayerController);
	
	// 관전자
	UPROPERTY(EditAnywhere)
	TSubclassOf<class ABlackholeSpectator> SpectatorPawnClass;
	void ConvertToSpectator(APlayerController* PlayerController);

	// 게임 시작과 종료 관련
	UPROPERTY(Replicated)
	float GameStartTime = 0.0f;
	// UPROPERTY(ReplicatedUsing = OnRep_GameEnded)
	// bool bGameEnded;

	// TODO: 팀 승리 로직
	//UPROPERTY(ReplicatedUsing = OnRep_GameEnded)
	//int32 WinningTeamId;

	// 멀티캐스트 RPC
	// UFUNCTION(NetMulticast, Reliable)
	// void Multicast_GameEnd(int32 TeamId);
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_PlayerDeath(APlayerController* PlayerController);
	// UFUNCTION(NetMulticast, Reliable)
	// void Multicast_SlowMotion(bool bEnable);

	// 리플리케이션 함수들
	/*UFUNCTION()
	void OnRep_PlayerDeathStates();*/
	/*UFUNCTION()
	void OnRep_TeamInfo();
	UFUNCTION()
	void OnRep_GameEnded();*/

	UPROPERTY(Replicated)
	class AWeaponSpawnManager* WeaponSpawnManager;
};
