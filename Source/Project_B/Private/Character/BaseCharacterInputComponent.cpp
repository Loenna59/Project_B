#include "Character/BaseCharacterInputComponent.h"

#include "Character/BaseCharacter.h"
#include "Character/BaseCharacterAnimInstance.h"

UBaseCharacterInputComponent::UBaseCharacterInputComponent()
{
	bWantsInitializeComponent = true;
	PrimaryComponentTick.bCanEverTick = false;

	SetIsReplicatedByDefault(true);
}

void UBaseCharacterInputComponent::InitializeComponent()
{
	Super::InitializeComponent();

	SetNetAddressable();
}

void UBaseCharacterInputComponent::BeginPlay()
{
	Super::BeginPlay();

	Character = Cast<ABaseCharacter>(GetOwner());

	if (Character)
	{
		AnimInstance = Cast<UBaseCharacterAnimInstance>(Character->GetMesh()->GetAnimInstance());
	}
}


