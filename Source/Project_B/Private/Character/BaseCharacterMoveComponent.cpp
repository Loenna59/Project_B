#include "Character/BaseCharacterMoveComponent.h"

#include "EnhancedInputComponent.h"
#include "InputAction.h"
#include "Character/BaseCharacter.h"

UBaseCharacterMoveComponent::UBaseCharacterMoveComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

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
}

void UBaseCharacterMoveComponent::SetupInputBiding(class UEnhancedInputComponent* input)
{
	Super::SetupInputBiding(input);

	input->BindAction(MoveInputAction, ETriggerEvent::Triggered, this, &UBaseCharacterMoveComponent::Move);
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

