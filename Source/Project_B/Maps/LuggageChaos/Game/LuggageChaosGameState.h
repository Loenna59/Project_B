// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Project_B/Maps/TeamMatchType.h"
#include "LuggageChaosGameState.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_B_API ALuggageChaosGameState : public AGameStateBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 MaxPoint = 24;

private:
	uint8 RedPoint = 0;
	uint8 BluePoint = 0;

public:
	void AddScore(ETeamType team ,const uint8 point);
	
private:
	void Win(ETeamType team);
};
