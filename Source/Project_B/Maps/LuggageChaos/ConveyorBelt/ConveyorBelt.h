// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ConveyorBelt.generated.h"

UCLASS()
class PROJECT_B_API AConveyorBelt : public AActor
{
	GENERATED_BODY()

public:
	AConveyorBelt();
	
	UPROPERTY(EditDefaultsOnly)
	class UArrowComponent* StartArrow;
	
	UPROPERTY(EditDefaultsOnly, Category = "Conveyor Belt")
	TArray<UStaticMeshComponent*> Plates;

	UPROPERTY(EditDefaultsOnly, Category = "Conveyor Belt")
	uint8 PlateCount = 16;

	FVector MoveDir;
	FVector StarLoc;
	FVector EndLoc;
	float MaxDist;
	
private:
	UPROPERTY()
	USceneComponent* Root;

	UPROPERTY()
	TArray<FVector> PlateInitPositions;


protected:
	virtual void BeginPlay() override;

	
};
