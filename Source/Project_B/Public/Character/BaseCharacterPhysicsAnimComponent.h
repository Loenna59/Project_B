// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "BaseCharacterPhysicsAnimComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECT_B_API UBaseCharacterPhysicsAnimComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UBaseCharacterPhysicsAnimComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	void TogglePhysicalAnimation();

protected:
	UPROPERTY()
	class ABaseCharacter* Character = nullptr;

	UPROPERTY()
	class USkeletalMeshComponent* Mesh = nullptr;

public:
	UPROPERTY()
	class UPhysicalAnimationComponent* PhysicalAnimationComp = nullptr;

	UPROPERTY(EditAnywhere)
	FName SimulateBoneName = TEXT("CharacterPelvis");

	UPROPERTY(EditAnywhere)
	float SimulateStrengthMultiplier = 100.f;
};
