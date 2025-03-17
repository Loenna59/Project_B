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
	if (Character->HasAuthority())
	{
		Multicast_Punch(PunchAnimMontage, ArmDirection);
	}
	else
	{
		Server_Punch(PunchAnimMontage, ArmDirection);
	}
	
	ArmDirection = ArmDirection == EArmDirection::LEFT? EArmDirection::RIGHT : EArmDirection::LEFT;
}

void UBaseCharacterAttackComponent::HeadButt()
{
	if (Character->HasAuthority())
	{
		Multicast_HeadButt(HeadButtAnimMontage);
		return;
	}

	Server_HeadButt(HeadButtAnimMontage);
}

void UBaseCharacterAttackComponent::Kick()
{
	if (Character->GetCharacterMovement()->IsFalling())
	{
		if (Character->HasAuthority())
		{
			Multicast_Kick(KickAnimMontage);
			return;
		}

		Server_Kick(KickAnimMontage);
	}
	
}

void UBaseCharacterAttackComponent::AddForceForwardVector()
{
	FString BoneName = ArmDirection == EArmDirection::LEFT? TEXT("UpperArm_L") : TEXT("UpperArm_R");
	
	FVector ForceDirection = Character->GetActorForwardVector() * ForceAmount;
	Character->GetMesh()->AddImpulseToAllBodiesBelow(ForceDirection, *BoneName, true);
}

void UBaseCharacterAttackComponent::Server_Punch_Implementation(UAnimMontage* Montage, EArmDirection Direction)
{
	Multicast_Punch(Montage, Direction);
}

bool UBaseCharacterAttackComponent::Server_Punch_Validate(UAnimMontage* Montage, EArmDirection Direction)
{
	return true;
}

void UBaseCharacterAttackComponent::Multicast_Punch_Implementation(UAnimMontage* Montage, EArmDirection Direction)
{
	if (Montage && Character)
	{
		FString BoneName = Direction == EArmDirection::LEFT? TEXT("UpperArm_L") : TEXT("UpperArm_R");
		Character->PlayAnimMontage(Montage, 1.f, *BoneName);
	}
}

void UBaseCharacterAttackComponent::Server_HeadButt_Implementation(UAnimMontage* Montage)
{
	Multicast_HeadButt(Montage);
}

bool UBaseCharacterAttackComponent::Server_HeadButt_Validate(UAnimMontage* Montage)
{
	return true;
}

void UBaseCharacterAttackComponent::Multicast_HeadButt_Implementation(UAnimMontage* Montage)
{
	if (Montage && Character)
	{
		Character->PlayAnimMontage(HeadButtAnimMontage, 1.5f);
	}
}

void UBaseCharacterAttackComponent::Server_Kick_Implementation(UAnimMontage* Montage)
{
	Multicast_Kick(Montage);
}

bool UBaseCharacterAttackComponent::Server_Kick_Validate(UAnimMontage* Montage)
{
	return true;
}

void UBaseCharacterAttackComponent::Multicast_Kick_Implementation(UAnimMontage* Montage)
{
	if (Montage && Character)
	{
		Character->PlayAnimMontage(KickAnimMontage, 2.f);
	}
}

