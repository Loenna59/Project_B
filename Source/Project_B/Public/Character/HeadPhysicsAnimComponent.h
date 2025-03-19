// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacterPhysicsAnimComponent.h"
#include "HeadPhysicsAnimComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECT_B_API UHeadPhysicsAnimComponent : public UBaseCharacterPhysicsAnimComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UHeadPhysicsAnimComponent();

protected:
	float Threshold = 0.01f;
	
	FVector PreviousVelocity;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};
