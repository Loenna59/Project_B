#include "Character/BaseCharacterPickComponent.h"

#include "EnhancedInputComponent.h"
#include "Character/BaseCharacter.h"
#include "Character/BaseCharacterAnimInstance.h"
#include "Character/BaseCharacterPhysicsAnimComponent.h"
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

void UBaseCharacterPickComponent::BeginPlay()
{
	Super::BeginPlay();

	HandLocation = Character->GetMesh()->GetBoneLocation(TEXT("Hand_R"));
}

void UBaseCharacterPickComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void UBaseCharacterPickComponent::SetupInputBiding(class UEnhancedInputComponent* input)
{
	Super::SetupInputBiding(input);

	input->BindAction(PickInputAction, ETriggerEvent::Started, this, &UBaseCharacterPickComponent::BeginPick);
	input->BindAction(PickInputAction, ETriggerEvent::Triggered, this, &UBaseCharacterPickComponent::Picking);
	input->BindAction(PickInputAction, ETriggerEvent::Completed, this, &UBaseCharacterPickComponent::ReleasePick);
}

void UBaseCharacterPickComponent::BeginPick()
{
	if (!Character || !AnimInstance)
	{
		return;
	}

}

void UBaseCharacterPickComponent::Picking()
{
	if (!Character || !AnimInstance)
	{
		return;
	}

	AnimInstance->bPicking = true;
	
	TWeakObjectPtr<UBaseCharacterPickComponent> ThisWeak = this;
	
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
		[ThisWeak] (bool bHit, FHitResult HitResult)
		{
			if (ThisWeak.IsValid())
			{
				ThisWeak->DetectNearby(bHit, HitResult.GetActor());
			}
		}
	);
}

void UBaseCharacterPickComponent::ReleasePick()
{
	UObject* obj = Character->GetDefaultSubobjectByName(TEXT("RightArmPhysicsAnimComp"));
	if (UBaseCharacterPhysicsAnimComponent* Right = Cast<UBaseCharacterPhysicsAnimComponent>(obj))
	{
		Right->TogglePhysicalAnimation(true);
	}

	if (AnimInstance)
	{
		AnimInstance->IKTargetLocation = HandLocation;
		AnimInstance->bPicking = false;
	}
}

void UBaseCharacterPickComponent::DetectNearby(bool bHit, AActor* Actor)
{
	if (Character)
	{
		if (AnimInstance)
		{
			if (bHit)
			{
				UObject* obj = Character->GetDefaultSubobjectByName(TEXT("RightArmPhysicsAnimComp"));
				if (UBaseCharacterPhysicsAnimComponent* Right = Cast<UBaseCharacterPhysicsAnimComponent>(obj))
				{
					Right->TogglePhysicalAnimation(false);
				}
				
				FVector TargetLocation = Actor->GetActorLocation();
			
				AnimInstance->IKTargetLocation = FMath::VInterpTo(
					AnimInstance->IKTargetLocation,
					TargetLocation, GetWorld()->DeltaTimeSeconds,
					2.5f
				);

				// LOG_SCREEN("DetectNearby %s", *Actor->GetActorNameOrLabel());
				return;
			}

			// LOG_SCREEN("DetectNearby None");
			
			AnimInstance->IKTargetLocation = HandLocation;
		}
	}
}

