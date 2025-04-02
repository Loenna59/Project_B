// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Project_B/Maps/BanimalsType.h"
#include "Project_B/Maps/Base/BanimalsGameState.h"
#include "LuggageChaosGameState.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_B_API ALuggageChaosGameState : public ABanimalsGameState
{
	GENERATED_BODY()
	
public:
	ALuggageChaosGameState();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Debug")
	bool isDummyPlayerInfo = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Debug")
	bool isAddScoreBlue = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Debug")
	bool isAddScoreRed = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Timer")
	float ReadyTime = 2.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Timer")
	float GameTime = 180.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Timer")
	float SlowTime = 0.2f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Timer")
	float EndTime = 1.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Timer")
	float RespawnTime = 10.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Timer")
	float DeadTime = 3.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category ="Game")
	uint8 MaxPoint = 24;

	ETeamType WinnerTeam = ETeamType::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Class | WinPrize")
	TSubclassOf<class AWinnerPrize> WinnerPrizeClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Class | UI")
	TSubclassOf<class ULuggageScoreWidget> ScoreWidgetClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Class | UI")
	TSubclassOf<class UGameReadyWidget> ReadyWidgetClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Class | UI")
	TSubclassOf<class UGameEndWidget> GameEndWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Class | UI")
	TSubclassOf<class UUserWidget> TransitionWidgetClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Class | Sound")
	USoundWave* BGM;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Class | Sound")
	USoundWave* SW_Win;

	UPROPERTY(Replicated)
	class AWeaponSpawnManager* WeaponSpawnManager;

private:
	FTimerHandle GameTimerHandle;
	
	FDelegateHandle OnPostLoadMapHandle;

	FLatentActionInfo LatenActionInfo;

	uint8 LoadComplete = 0;

	// 플레이어 unique ID, Info
	TMap<FString,FPlayerInfo> PlayersInfo;

	UPROPERTY(Replicated)
	TArray<FString> WinnerKeys;

	TQueue<APlayerState*> DeadPlayers;
	TQueue<APawn*> DeadPawns;

	FString MyKey = "";
	
	int32 dummyKey = 0;
	
	// 플레이어 초기 위치 초기화를 위한 변수
	UPROPERTY()
	TArray<AActor*> BlueSpawnPoints;
	UPROPERTY()
	TArray<AActor*> RedSpawnPoints;

	int32 blueIdx = 0;
	int32 redIdx = 0;
	
	uint8 RedPoint = 0;
	uint8 BluePoint = 0;
	
	UPROPERTY()
	ULuggageScoreWidget* ScoreWidget;
	UPROPERTY()
	UGameReadyWidget* GameReadyWidget;
	UPROPERTY()
	UGameEndWidget* GameEndWidget;
	UPROPERTY()
	UUserWidget* TransitionWidget;

	UPROPERTY()
	UAudioComponent* BgmComponent;

protected:
	virtual void BeginPlay() override;
	
public:
	/**모든 클라이언트에서 호출되어야 함*/
	void AddScore(ETeamType team ,const uint8 point);
	
	void AddWinner(FString playerKey);
	
	void InitPlayerLoc(APawn* pawn,FString key);

	UFUNCTION(BlueprintCallable)
	void OnPlayerDeath(APlayerController* pc);

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	void GameReady();
	
	void InitPlayerInfo();
	void InitSpawnPoint();
	void InitUI(APlayerController* pc);

	UFUNCTION(NetMulticast, Reliable)
	void Net_GameStart();
	void GameStart();
	
	void TimeOut();

	UFUNCTION(NetMulticast, Reliable)
	void Net_JudgeWinner();
	
	void Win(ETeamType winner = ETeamType::None);
	void AddWinPrize(APlayerController* pc);
	
	UFUNCTION(NetMulticast, Reliable)
	void Net_GameEnd();
	void GameEnd();
	
	void ChangeLevelPodium();

	UFUNCTION(Server, Reliable)
	void Server_OnPlayerDeath(APlayerController* pc);
	UFUNCTION(NetMulticast, Reliable)
	void Net_OnPlayerDeath(ABaseCharacter* player, APlayerController* pc);

	void AddDeadPlayer(APlayerController* pc);
	
	void DeathEffects(APlayerController* pc);
	
	UFUNCTION(NetMulticast, Reliable)
	void Net_OnPlayerSpectate(ABaseCharacter* player, APlayerController* pc);
	
	UFUNCTION(NetMulticast, Reliable)
	void Net_OnPlayerRespawn(APlayerController* pc, ABaseCharacter* player);
	void Respawn();

	TMap<FString,FPlayerInfo> DummyPlayersInfo();
};
