// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Project_B/Project_BGameMode.h"
#include "LuggageChaosGameMode.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_B_API ALuggageChaosGameMode : public AProject_BGameMode
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 MaxPoint = 24;

private:
	uint8 RedPoint = 0;
	uint8 BluePoint = 0;

public:
	void AddScoreRed(const uint8 point); // { RedPoint += point; }
	void AddScoreBlue(const uint8 point); //{ BluePoint += point; }
};
