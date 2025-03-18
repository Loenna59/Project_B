// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseLeverInteractor.generated.h"

UCLASS()
class PROJECT_B_API ABaseLeverInteractor : public AActor
{
	GENERATED_BODY()

public:
	ABaseLeverInteractor();

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category=Lever)
	class ALever* Lever;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UStaticMeshComponent* StaticMesh;

private:
	float LeverValue;

	UPROPERTY(EditAnywhere)
	USceneComponent* Root;

protected:
	virtual void BeginPlay() override;

public:

	UFUNCTION()
	virtual void LeverValueInteract(float value);
};
