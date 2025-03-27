// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Project_B/Project_BGameMode.h"
#include "Project_B/Maps/BanimalsType.h"
#include "LuggageChaosGameMode.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_B_API ALuggageChaosGameMode : public AProject_BGameMode
{
	GENERATED_BODY()
private:
	virtual void OnPostLogin(AController* NewPlayer) override;
};
