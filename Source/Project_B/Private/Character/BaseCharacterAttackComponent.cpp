#include "Character/BaseCharacterAttackComponent.h"

#include "EnhancedInputComponent.h"
#include "Character/BaseCharacter.h"
#include "Project_B/Utilities/LogMacro.h"


UBaseCharacterAttackComponent::UBaseCharacterAttackComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	ConstructorHelpers::FObjectFinder<UInputAction> tmp_ia_punch(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_Punch.IA_Punch'"));

	if (tmp_ia_punch.Succeeded())
	{
		PunchInputAction = tmp_ia_punch.Object;
	}
}


void UBaseCharacterAttackComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void UBaseCharacterAttackComponent::SetupInputBiding(class UEnhancedInputComponent* input)
{
	Super::SetupInputBiding(input);

	input->BindAction(PunchInputAction, ETriggerEvent::Started, this, &UBaseCharacterAttackComponent::Punch);
}

void UBaseCharacterAttackComponent::Punch()
{
	if (Character)
	{
		FString DirString = ArmDirection == EArmDirection::LEFT? TEXT("Left") : TEXT("Right");
		Character->PlayAnimMontage(PunchAnimMontage, 2.f, *DirString);
		
		ArmDirection = ArmDirection == EArmDirection::LEFT? EArmDirection::RIGHT : EArmDirection::LEFT;
	}
}

