#include "Character/BaseCharacterPickComponent.h"

#include "EnhancedInputComponent.h"
#include "Character/BaseCharacter.h"
#include "Character/BaseCharacterArmComponent.h"

UBaseCharacterPickComponent::UBaseCharacterPickComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	SetIsReplicatedByDefault(true);

	ConstructorHelpers::FObjectFinder<UInputAction> tmp_ia(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_Pick.IA_Pick'"));

	if (tmp_ia.Succeeded())
	{
		PickInputAction = tmp_ia.Object;
	}
}

void UBaseCharacterPickComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void UBaseCharacterPickComponent::SetupInputBiding(class UEnhancedInputComponent* input)
{
	Super::SetupInputBiding(input);
	
	input->BindAction(PickInputAction, ETriggerEvent::Triggered, this, &UBaseCharacterPickComponent::Picking);
	input->BindAction(PickInputAction, ETriggerEvent::Completed, this, &UBaseCharacterPickComponent::ReleasePick);
}

void UBaseCharacterPickComponent::Picking()
{
	if (OnGrabbing.IsBound())
	{
		OnGrabbing.Broadcast();
	}
}

void UBaseCharacterPickComponent::ReleasePick()
{
	if (OnRelease.IsBound())
	{
		OnRelease.Broadcast();
	}
}

