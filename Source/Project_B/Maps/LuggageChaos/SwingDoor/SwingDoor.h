// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SwingDoor.generated.h"

class UBoxComponent;
class UPhysicsConstraintComponent;

UCLASS()
class PROJECT_B_API ASwingDoor : public AActor
{
	GENERATED_BODY()

public:
	ASwingDoor();

	UPROPERTY()
	USceneComponent* Root;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SwingDoor")
	UStaticMeshComponent* LeftDoor;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SwingDoor")
	UStaticMeshComponent* RightDoor;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SwingDoor")
	UStaticMeshComponent* LeftGlass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SwingDoor")
	UStaticMeshComponent* RightGlass;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SwingDoor")
	USceneComponent* LeftPivot;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SwingDoor")
	USceneComponent* RightPivot;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SwingDoor")
	UPhysicsConstraintComponent* LeftConstraint;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SwingDoor")
	UPhysicsConstraintComponent* RightConstraint;

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
};
