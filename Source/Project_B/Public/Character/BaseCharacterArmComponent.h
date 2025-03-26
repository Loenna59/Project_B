// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacterPhysicsAnimComponent.h"
#include "BaseCharacterArmComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECT_B_API UBaseCharacterArmComponent : public UBaseCharacterPhysicsAnimComponent
{
	GENERATED_BODY()

public:
	UBaseCharacterArmComponent();

protected:
	virtual void BeginPlay() override;

	UPROPERTY()
	bool bIsAttached = false;

	UPROPERTY()
	class UPrimitiveComponent* GrabComponent = nullptr;

	UPROPERTY()
	class UPhysicsConstraintComponent* GrabConstraintComp = nullptr;
	
public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	void BeginGrab();

	UFUNCTION()
	void Grabbing();

	UFUNCTION()
	void ReleaseGrab();

	UFUNCTION()
	void DetectNearby(bool bHit, FHitResult HitResult);

	UFUNCTION()
	void AttachTo(UPrimitiveComponent* Comp, FVector Location, FRotator Rotation);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Radius = 20;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttachDistanceThreshold = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName SocketName = TEXT("Hand");

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName BoneName = TEXT("Hand_R");
};
