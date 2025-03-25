// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Project_B/Project_BGameMode.h"
#include "BlackholeGameMode.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_B_API ABlackholeGameMode : public AProject_BGameMode
{
	GENERATED_BODY()
public:
	virtual void PostLogin(APlayerController* NewPlayer) override;
	
public:
	int32 playerIdx = 0;
};
