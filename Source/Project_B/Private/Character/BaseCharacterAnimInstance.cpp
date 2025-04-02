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

	bIsDead = Character->bIsDead;

	if (Character->bIsDead)
	{
		return;
	}

	Velocity = MovementComponent->Velocity;
	GroundSpeed = Velocity.Size2D();

	FVector Acceleration = MovementComponent->GetCurrentAcceleration();
	
	bShouldMove = Acceleration != FVector::ZeroVector && GroundSpeed > 3.f;
	bIsFalling = MovementComponent->IsFalling();

	float TargetAlpha_L = bIsGrabbing[EGrabState::Left]? 1.f : 0.f;
	HandIKAlpha[EGrabState::Left] = FMath::FInterpTo(HandIKAlpha[EGrabState::Left], TargetAlpha_L, DeltaSeconds, 5.f);

	float TargetAlpha_R = bIsGrabbing[EGrabState::Right]? 1.f : 0.f;
	HandIKAlpha[EGrabState::Right] = FMath::FInterpTo(HandIKAlpha[EGrabState::Right], TargetAlpha_R, DeltaSeconds, 5.f);
}
