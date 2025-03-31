#include "Character/BaseCharacterPickComponent.h"
#include "EnhancedInputComponent.h"
#include "Project_B/Utilities/LogMacro.h"

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

	GrabState = EGrabState::None;
	BlockGrabState = EGrabState::None;
}

void UBaseCharacterPickComponent::UpdateGrabState(EGrabState State, bool bIsGrabbing)
{
	if (BlockGrabState == State)
	{
		return;
	}

	const UEnum* EnumPtr = StaticEnum<EGrabState>();
	
	EGrabState Current = GrabState;

	if (bIsGrabbing)
	{
		if (static_cast<bool>(Current & State))
		{
			return;
		}
		
		GrabState |= State;
		
		LOG_SCREEN("Grab %s", *EnumPtr->GetDisplayNameTextByValue(static_cast<uint8>(GrabState)).ToString());
	}
	else
	{
		if (!static_cast<bool>(Current & State))
		{
			return;
		}
		
		GrabState &= ~State;
		
		LOG_SCREEN("Rlease %s", *EnumPtr->GetDisplayNameTextByValue(static_cast<uint8>(GrabState)).ToString());
	}

	GetWorld()->GetTimerManager().ClearTimer(ValidTimerHandle);
	
	if (Current == EGrabState::None && GrabState < EGrabState::Both)
	{
		GetWorld()->
		GetTimerManager()
		.SetTimer(
			ValidTimerHandle,
			this,
			&UBaseCharacterPickComponent::ValidateGrabState,
			ValidationThreshold
		);
	}
}

void UBaseCharacterPickComponent::ValidateGrabState()
{
	if (GrabState > EGrabState::None)
	{
		BlockGrabState = ~GrabState & EGrabState::Both;

		const UEnum* EnumPtr = StaticEnum<EGrabState>();
		LOG_SCREEN("Set Block %s", *EnumPtr->GetDisplayNameTextByValue(static_cast<uint8>(BlockGrabState)).ToString());

		OffPhysics.Broadcast(BlockGrabState);
		return;
	}

	BlockGrabState = EGrabState::None;
}
