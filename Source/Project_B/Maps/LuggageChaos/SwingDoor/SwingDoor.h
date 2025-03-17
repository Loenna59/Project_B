// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SwingDoor.generated.h"

class USphereComponent;
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

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UBoxComponent* BoxCollison;

private:
	bool bIsLugNear = false;
	uint8 NearLugCount = 0;

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	UFUNCTION()
	void OnCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:
	void DoorLocked();
	void DoorUnlocked();
};
