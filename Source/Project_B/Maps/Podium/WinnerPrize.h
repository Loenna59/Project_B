// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WinnerPrize.generated.h"

UCLASS()
class PROJECT_B_API AWinnerPrize : public AActor
{
	GENERATED_BODY()

public:
	AWinnerPrize();

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	USkeletalMeshComponent* SkeletalMesh;
	
protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
};
