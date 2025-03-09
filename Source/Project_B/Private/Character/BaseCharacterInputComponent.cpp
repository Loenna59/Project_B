#include "Character/BaseCharacterInputComponent.h"

#include "Character/BaseCharacter.h"
#include "Character/BaseCharacterAnimInstance.h"

UBaseCharacterInputComponent::UBaseCharacterInputComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
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


