// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Project_B/Maps/LuggageChaos/Lever/BaseLeverInteractor.h"
#include "LeverDoor.generated.h"

UCLASS()
class PROJECT_B_API ALeverDoor : public ABaseLeverInteractor
{
	GENERATED_BODY()

public:
	ALeverDoor();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void LeverValueInteract(float value) override;
};
