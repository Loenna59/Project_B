// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Luggage.generated.h"

UCLASS()
class PROJECT_B_API ALuggage : public AActor
{
	GENERATED_BODY()

public:
	ALuggage();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* Root;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* LuggageMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Luggage")
	uint8 Point = 1;

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
};
