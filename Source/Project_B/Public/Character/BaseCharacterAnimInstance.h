// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BaseCharacterAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_B_API UBaseCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
protected:
	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class ABaseCharacter* Character;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UCharacterMovementComponent* MovementComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FVector Velocity;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float GroundSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bShouldMove;
};
