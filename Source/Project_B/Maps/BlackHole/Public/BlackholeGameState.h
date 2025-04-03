// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/BaseCharacter.h"
#include "GameFramework/GameStateBase.h"
#include "Project_B/Maps/BanimalsType.h"
#include "BlackholeGameState.generated.h"


enum class ETeamType : uint8;
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
	
	UPROPERTY(Replicated)
	float GameStartTime = 0.0f;

	// 게임 시작
	void GameReady();
	void StartGame();
	UFUNCTION(NetMulticast, reliable)
	void MulticastRPC_GameStart();
	void InitPlayerInfo();
	FString MyKey;

	// 게임 종료
	void GameEnd();
	UFUNCTION(NetMulticast, reliable)
	void MulticastRPC_GameEnd();

	// 관련 위젯
	void InitUI(APlayerController* pc);
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UGameReadyWidget> ReadyWidgetClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UGameEndWidget> GameEndWidgetClass;
	UPROPERTY()
	UGameReadyWidget* GameReadyWidget;
	UPROPERTY()
	UGameEndWidget* GameEndWidget;
	
	// 블랙홀 관련 내용
	ADestroyZone* Rotator = nullptr; // 회전체
	UPROPERTY(Replicated)
	ABlackHole* Blackhole = nullptr; // 블랙홀
	void SpawnBlackhole();
	void DestroyBlackhole();

	UFUNCTION(NetMulticast, reliable)
	void Multicast_OnBlackholeSpawned();
	UFUNCTION(NetMulticast, reliable)
	void Multicast_OnBlackholeDestryed();
	
	// 블랙홀 타이머 (게임시작->30초후, 페이즈완전히 종료 30초후)
	FTimerHandle BlackholeSpawnHandle;
	FTimerHandle BlackholeDestroyHandle;
	
	// 블랙홀 소환 횟수
	UPROPERTY(Replicated)
	int32 BlackholeSpawnCount = 0;

	
	// 게임 조건 체크
	// 게임 승리/패배 결정을 위한 생존 플레이어 수 체크
	UPROPERTY(Replicated, VisibleAnywhere)
	int32 AlivePlayers = 0;

	// Replicated 변수 추가
	UPROPERTY(Replicated = OnRep_PlayerDeathStates)
	TArray<APlayerController*> DeadPlayers;
	void AddDeadPlayer(APlayerController* PlayerController);
	
	// 게임 종료 조건 확인
	UFUNCTION()
	void CheckGameEndConditions();
	
	// 승자 결정
	TMap<FString, FPlayerInfo> PlayersInfo;
	UPROPERTY(Replicated)
	TArray<FString> WinnerKeys;
	ETeamType WinnerTeam = ETeamType::None;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class AWinnerPrize> WinnerPrizeClass;
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_DetermineTeamWinner(ETeamType WinningTeam);
	void AddWinPrize(APlayerController* pc);
	void AddWinner(FString playerKey);
	
	// 죽음 처리
	void OnPlayerDeath(APlayerController* PlayerController);
	void DeathEffects(APlayerController* PlayerController);
	UFUNCTION(Server, Reliable)
	void ServerRPC_PlayerDeath(APlayerController* PlayerController);
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_PlayerDeath(APlayerController* PlayerController);
	
	// 관전자
	UPROPERTY(EditAnywhere)
	TSubclassOf<class ABlackholeSpectator> SpectatorPawnClass;
	void ConvertToSpectator(APlayerController* PlayerController);
	
	// 레벨 전환
	void ChangeLevelPodium();
	
	// 무기 소환
	UPROPERTY(Replicated)
	class AWeaponSpawnManager* WeaponSpawnManager;

	// 스킨 설정 로직
	void ApplySkin(FString PlayerKey, ETeamType Team);
	UFUNCTION(netmulticast, reliable)
	void Multicast_ApplyPlayerSkin(ABaseCharacter* player, CharacterColor Color);
	UFUNCTION(Server, Reliable)
	void ServerRPC_ApplyPlayerSkin();
};


