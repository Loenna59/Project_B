#include "Character/BaseCharacterAttackComponent.h"

#include "EnhancedInputComponent.h"
#include "Character/BaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"
#include "Project_B/Utilities/LogMacro.h"


UBaseCharacterAttackComponent::UBaseCharacterAttackComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);

	ConstructorHelpers::FObjectFinder<UInputAction> tmp_ia_punch(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_Punch.IA_Punch'"));

	if (tmp_ia_punch.Succeeded())
	{
		PunchInputAction = tmp_ia_punch.Object;
	}

	ConstructorHelpers::FObjectFinder<UInputAction> tmp_ia_hb(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_HeadButt.IA_HeadButt'"));

	if (tmp_ia_hb.Succeeded())
	{
		HeadButtInputAction = tmp_ia_hb.Object;
	}

	ConstructorHelpers::FObjectFinder<UInputAction> tmp_ia_kick(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_Kick.IA_Kick'"));

	if (tmp_ia_kick.Succeeded())
	{
		KickInputAction = tmp_ia_kick.Object;
	}
}

void UBaseCharacterAttackComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void UBaseCharacterAttackComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UBaseCharacterAttackComponent, ArmDirection);
	DOREPLIFETIME(UBaseCharacterAttackComponent, PunchAnimMontage);
	DOREPLIFETIME(UBaseCharacterAttackComponent, HeadButtAnimMontage);
	DOREPLIFETIME(UBaseCharacterAttackComponent, KickAnimMontage);
}

void UBaseCharacterAttackComponent::SetupInputBiding(class UEnhancedInputComponent* input)
{
	Super::SetupInputBiding(input);

	input->BindAction(PunchInputAction, ETriggerEvent::Started, this, &UBaseCharacterAttackComponent::Punch);
	input->BindAction(HeadButtInputAction, ETriggerEvent::Started, this, &UBaseCharacterAttackComponent::HeadButt);
	input->BindAction(KickInputAction, ETriggerEvent::Started, this, &UBaseCharacterAttackComponent::Kick);
}

void UBaseCharacterAttackComponent::Punch()
{
	if (!Character)
	{
		return;
	}
	
	FString BoneName = ArmDirection == EArmDirection::LEFT? TEXT("UpperArm_L") : TEXT("UpperArm_R");
	
	if (Character->HasAuthority())
	{
		Multicast_PlayAnimMontage(PunchAnimMontage, 1.f, *BoneName);
	}
	else
	{
		Server_PlayAnimMontage(PunchAnimMontage, 1.f, *BoneName);
	}
	
	ArmDirection = ArmDirection == EArmDirection::LEFT? EArmDirection::RIGHT : EArmDirection::LEFT;
}

void UBaseCharacterAttackComponent::HeadButt()
{
	if (!Character)
	{
		return;
	}
	
	if (Character->HasAuthority())
	{
		Multicast_PlayAnimMontage(HeadButtAnimMontage, 1.5f);
		return;
	}

	Server_PlayAnimMontage(HeadButtAnimMontage, 1.5f);
}

void UBaseCharacterAttackComponent::Kick()
{
	if (!Character)
	{
		return;
	}
	
	if (Character->GetCharacterMovement()->IsFalling())
	{
		if (Character->HasAuthority())
		{
			Multicast_PlayAnimMontage(KickAnimMontage, 1.5f);
			return;
		}

		Server_PlayAnimMontage(KickAnimMontage, 1.5f);
	}
	
}

void UBaseCharacterAttackComponent::Server_PlayAnimMontage_Implementation(UAnimMontage* Montage, float PlayRate,
                                                                          FName SectionName)
{
	Multicast_PlayAnimMontage(Montage, PlayRate, SectionName);
}

void UBaseCharacterAttackComponent::Multicast_PlayAnimMontage_Implementation(UAnimMontage* Montage, float PlayRate,
                                                                             FName SectionName)
{
	if (Montage && Character)
	{
		Character->PlayAnimMontage(Montage, PlayRate, SectionName);
	}
}

void UBaseCharacterAttackComponent::AddForceForwardVector()
{
	FString BoneName = ArmDirection == EArmDirection::LEFT? TEXT("UpperArm_L") : TEXT("UpperArm_R");
	
	FVector ForceDirection = Character->GetActorForwardVector() * ForceAmount;
	Character->GetMesh()->AddImpulseToAllBodiesBelow(ForceDirection, *BoneName, true);
}
