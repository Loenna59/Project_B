// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/BaseCharacterAnimInstance.h"

#include "Character/BaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void UBaseCharacterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Character = Cast<ABaseCharacter>(TryGetPawnOwner());

	if (Character)
	{
		MovementComponent = Character->GetCharacterMovement();
	}
}

void UBaseCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!Character)
	{
		return;
	}

	Velocity = MovementComponent->Velocity;
	GroundSpeed = Velocity.Size2D();

	FVector Acceleration = MovementComponent->GetCurrentAcceleration();
	
	bShouldMove = Acceleration != FVector::ZeroVector && GroundSpeed > 3.f;
}
