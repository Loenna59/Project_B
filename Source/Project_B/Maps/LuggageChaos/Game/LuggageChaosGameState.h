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
	float GameTime = 6.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Timer")
	float SlowTime = 0.2f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Timer")
	float EndTime = 1.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Timer")
	float RespawnTime = 3.0f;
	
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

	FString MyKey = "";
	
	// 플레이어 초기 위치 초기화를 위한 함수
	TArray<AActor*> BlueSpawnPoints;
	TArray<AActor*> RedSpawnPoints;
	int32 dummyKey = 0;

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

protected:
	virtual void BeginPlay() override;
	
public:
	/**모든 클라이언트에서 호출되어야 함*/
	void AddScore(ETeamType team ,const uint8 point);
	void AddWinner(FString playerKey);
	void InitPlayerLoc(APawn* pawn,FString key);
 
protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	void GameReady();
	
	void InitPlayerInfo();
	
	void InitSpawnPoint();
	
	void InitUI(APlayerController* pc);

	void GameStart();
	
	UFUNCTION(NetMulticast, Reliable)
	void Net_GameStart();
	
	void TimeOut();

	UFUNCTION(NetMulticast, Reliable)
	void Net_JudgeWinner();
	
	void Win(ETeamType winner = ETeamType::None);
	
	void AddWinPrize(APlayerController* pc);
	
	void GameEnd();
	
	UFUNCTION(NetMulticast, Reliable)
	void Net_GameEnd();
	
	void ChangeLevelPodium();

	TMap<FString,FPlayerInfo> DummyPlayersInfo();
};
