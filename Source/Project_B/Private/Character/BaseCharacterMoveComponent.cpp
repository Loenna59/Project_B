#include "Character/BaseCharacterMoveComponent.h"

#include "EnhancedInputComponent.h"
#include "InputAction.h"
#include "Character/BaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"

UBaseCharacterMoveComponent::UBaseCharacterMoveComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);

	ConstructorHelpers::FObjectFinder<UInputAction> tmp_ia_move(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_Move.IA_Move'"));

	if (tmp_ia_move.Succeeded())
	{
		MoveInputAction = tmp_ia_move.Object;
	}

	ConstructorHelpers::FObjectFinder<UInputAction> tmp_ia_rotate(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_Look.IA_Look'"));

	if (tmp_ia_rotate.Succeeded())
	{
		RotateInputAction = tmp_ia_rotate.Object;
	}

	ConstructorHelpers::FObjectFinder<UInputAction> tmp_ia_jump(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_Jump.IA_Jump'"));

	if (tmp_ia_jump.Succeeded())
	{
		JumpInputAction = tmp_ia_jump.Object;
	}

	ConstructorHelpers::FObjectFinder<UInputAction> tmp_ia_run(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_Run.IA_Run'"));

	if (tmp_ia_run.Succeeded())
	{
		RunInputAction = tmp_ia_run.Object;
	}
}

void UBaseCharacterMoveComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UBaseCharacterMoveComponent, ReplicatedSpeed);
}

void UBaseCharacterMoveComponent::BeginPlay()
{
	Super::BeginPlay();

	if (Character)
	{
		Character->GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	}
}

void UBaseCharacterMoveComponent::SetupInputBiding(class UEnhancedInputComponent* input)
{
	Super::SetupInputBiding(input);

	input->BindAction(MoveInputAction, ETriggerEvent::Triggered, this, &UBaseCharacterMoveComponent::Move);
	input->BindAction(RotateInputAction, ETriggerEvent::Triggered, this, &UBaseCharacterMoveComponent::Rotate);

	input->BindAction(JumpInputAction, ETriggerEvent::Started, this, &UBaseCharacterMoveComponent::StartJump);
	input->BindAction(JumpInputAction, ETriggerEvent::Completed, this, &UBaseCharacterMoveComponent::EndJump);

	input->BindAction(RunInputAction, ETriggerEvent::Started, this, &UBaseCharacterMoveComponent::StartRun);
	input->BindAction(RunInputAction, ETriggerEvent::Completed, this, &UBaseCharacterMoveComponent::EndRun);
}

void UBaseCharacterMoveComponent::Move(const FInputActionValue& actionValue)
{
	FVector2D value = actionValue.Get<FVector2D>();

	if (Character)
	{
		FVector toVector(value.Y, value.X, 0);
		FTransform controlTransform(Character->GetControlRotation());
		
		Character->AddMovementInput(controlTransform.TransformVector(toVector));
	}
}

void UBaseCharacterMoveComponent::Rotate(const FInputActionValue& actionValue)
{
	FVector2D LookAxisVector = actionValue.Get<FVector2D>();

	if (Character)
	{
		Character->AddControllerYawInput(LookAxisVector.X);
		Character->AddControllerPitchInput(LookAxisVector.Y);
	}
}

void UBaseCharacterMoveComponent::StartJump()
{
	if (Character)
	{
		Character->Jump();
	}
}

void UBaseCharacterMoveComponent::EndJump()
{
	if (Character)
	{
		Character->StopJumping();
	}
}

void UBaseCharacterMoveComponent::StartRun()
{
	UpdateSpeed(RunSpeed);
}

void UBaseCharacterMoveComponent::EndRun()
{
	UpdateSpeed(WalkSpeed);
}

void UBaseCharacterMoveComponent::Server_UpdateSpeed_Implementation(float Speed)
{
	Multicast_UpdateSpeed(Speed);
}

void UBaseCharacterMoveComponent::Multicast_UpdateSpeed_Implementation(float Speed)
{
	Character->GetCharacterMovement()->MaxWalkSpeed = Speed;
}

void UBaseCharacterMoveComponent::UpdateSpeed(float Speed)
{
	if (Character->HasAuthority())
	{
		Multicast_UpdateSpeed(Speed);
		return;
	}

	Server_UpdateSpeed(Speed);
}

