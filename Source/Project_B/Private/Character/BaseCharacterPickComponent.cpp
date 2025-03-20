#include "Character/BaseCharacterPickComponent.h"

#include "EnhancedInputComponent.h"
#include "Character/BaseCharacter.h"
#include "Project_B/Utilities/LogMacro.h"
#include "Project_B/Utilities/TraceChannelHelper.h"

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

	input->BindAction(PickInputAction, ETriggerEvent::Triggered, this, &UBaseCharacterPickComponent::BeginPick);
}

void UBaseCharacterPickComponent::BeginPick(const FInputActionValue& actionValue)
{
	if (!Character)
	{
		return;
	}
	
	TraceChannelHelper::SphereSingleByChannel
	(
		GetWorld(),
		Character,
		Character->GetActorLocation(),
		Character->GetActorLocation(),
		FRotator::ZeroRotator,
		ECC_Visibility,
		Radius,
		true,
		true,
		[] (bool bHit, FHitResult HitResult)
		{
			
		}
	);
}

