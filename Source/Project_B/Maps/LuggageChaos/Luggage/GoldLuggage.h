// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Luggage.h"
#include "GoldLuggage.generated.h"

UCLASS()
class PROJECT_B_API AGoldLuggage : public ALuggage
{
	GENERATED_BODY()

public:
	AGoldLuggage();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
};
