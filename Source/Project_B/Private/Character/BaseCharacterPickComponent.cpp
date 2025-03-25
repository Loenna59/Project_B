#include "Character/BaseCharacterPickComponent.h"

#include "EnhancedInputComponent.h"
#include "Character/BaseCharacter.h"
#include "Character/BaseCharacterArmComponent.h"
#include "Character/BaseCharacterPhysicsAnimComponent.h"

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

	if (!Character)
	{
		return;
	}
	
	LeftArmComp = Cast<UBaseCharacterArmComponent>(Character->GetDefaultSubobjectByName(TEXT("LeftArmPhysicsAnimComp")));
	RightArmComp = Cast<UBaseCharacterArmComponent>(Character->GetDefaultSubobjectByName(TEXT("RightArmPhysicsAnimComp")));
}

void UBaseCharacterPickComponent::TickComponent(float DeltaTime, enum ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	
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
	if (RightArmComp)
	{
		RightArmComp->BeginGrab();
	}
}

void UBaseCharacterPickComponent::Picking()
{
	if (RightArmComp)
	{
		RightArmComp->Grabbing();
	}
}

void UBaseCharacterPickComponent::ReleasePick()
{
	if (RightArmComp)
	{
		RightArmComp->ReleaseGrab();
	}
}

