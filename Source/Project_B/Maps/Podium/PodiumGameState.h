// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Project_B/Maps/BanimalsType.h"
#include "PodiumGameState.generated.h"

class UPodiumMainWidget;
class UPictureWidget;
/**
 * 
 */
UCLASS()
class PROJECT_B_API APodiumGameState : public AGameStateBase
{
	GENERATED_BODY()
public:
	APodiumGameState();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug")
	bool isDummyPlayer = false;
	
	UPROPERTY()
	class APodiumCamera* PodiumCamera = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timer")
	float ReadyTime = 12.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Class | WinPrize")
	TSubclassOf<class AWinnerPrize> WinnerPrizeClass;

private:
	TMap<FString,FPlayerInfo> PlayersInfo;

	UPROPERTY()
	TArray<AActor*> WinnerPoints1;
	UPROPERTY()
	TArray<AActor*> WinnerPoints2;
	UPROPERTY()
	TArray<AActor*> NormalPoints1;
	UPROPERTY()
	TArray<AActor*> NormalPoints2;

	uint8 WinIndex = 0;
	uint8 NorIndex = 0;

	FString mykey;
	uint8 dummyKey = 0;
	
protected:
	virtual void BeginPlay() override;

public:
	void InitPlayerLoc(APawn* pawn,FString key);

	void AddWinPrize(APawn* pawn);
	
	UFUNCTION(NetMulticast, Reliable)
	void Net_AddWinPrize(APawn* pawn);
	
	void InitPodiumCamera(APlayerController* pc);

private:
	void InitSpawnPoints();
	
	UFUNCTION(NetMulticast, Reliable)
	void Net_Shoot();

	void Shoot();

	TMap<FString,FPlayerInfo> DummyPlayersInfo();
};
