#include "Character/BaseCharacterAttackComponent.h"

#include "EnhancedInputComponent.h"
#include "Character/BaseCharacter.h"
#include "Character/BaseCharacterAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"
#include "Project_B/Utilities/LogMacro.h"
#include "Project_B/Utilities/TraceChannelHelper.h"

UBaseCharacterAttackComponent::UBaseCharacterAttackComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
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

void UBaseCharacterAttackComponent::TickComponent(float DeltaTime, enum ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bBeginPunchInput)
	{
		if (PunchPressingTime > PunchExecuteThreshold)
		{
			return;
		}
		
		PunchPressingTime += DeltaTime;
	}
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

	input->BindAction(PunchInputAction, ETriggerEvent::Started, this, &UBaseCharacterAttackComponent::BeginPunch);
	input->BindAction(PunchInputAction, ETriggerEvent::Completed, this, &UBaseCharacterAttackComponent::Punch);
	input->BindAction(HeadButtInputAction, ETriggerEvent::Started, this, &UBaseCharacterAttackComponent::HeadButt);
	input->BindAction(KickInputAction, ETriggerEvent::Started, this, &UBaseCharacterAttackComponent::Kick);
}

void UBaseCharacterAttackComponent::BeginPunch()
{
	if (bBeginPunchInput)
	{
		return;
	}

	PunchPressingTime = 0;
	bBeginPunchInput = true;
}

void UBaseCharacterAttackComponent::Punch()
{
	bBeginPunchInput = false;
	
	if (PunchPressingTime > PunchExecuteThreshold)
	{
		return;
	}
	
	if (!Character)
	{
		return;
	}

	if (bIsAttacking)
	{
		return;
	}

	bIsAttacking = true;
	
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

	if (bIsAttacking)
	{
		return;
	}

	bIsAttacking = true;
	
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
		if (bIsAttacking)
		{
			return;
		}

		bIsAttacking = true;
		
		if (Character->HasAuthority())
		{
			Multicast_PlayAnimMontage(KickAnimMontage, 1.5f);
			return;
		}

		Server_PlayAnimMontage(KickAnimMontage, 1.5f);
	}
	
}

void UBaseCharacterAttackComponent::Finish()
{
	bIsAttacking = false;

	AlreadyHitActorsDuringAttack.Empty();
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

void UBaseCharacterAttackComponent::OnPunchTraceChannel()
{
	Server_OnPunchTraceChannel();
}

void UBaseCharacterAttackComponent::Server_OnPunchTraceChannel_Implementation()
{
	FName BoneName = ArmDirection == EArmDirection::LEFT? TEXT("Hand_R") : TEXT("Hand_L");

	FVector Location = Character->GetMesh()->GetBoneLocation(BoneName);

	TWeakObjectPtr<UBaseCharacterAttackComponent> WeakThis = this;

	TraceChannelHelper::SphereSingleByChannel
	(
		GetWorld(),
		Character,
		Location,
		Location,
		FRotator::ZeroRotator,
		ECC_Camera,
		20.f,
		true,
		true,
		[WeakThis] (bool bHit, FHitResult HitResult)
		{
			if (WeakThis.IsValid())
			{
				WeakThis->Multicast_OnHitTraceChannel_Implementation(bHit, HitResult, 10);
			}
		}
	);
}

void UBaseCharacterAttackComponent::OnKickTraceChannel()
{
	Server_OnKickTraceChannel();
}

void UBaseCharacterAttackComponent::Server_OnKickTraceChannel_Implementation()
{
	FName BoneName = TEXT("FootToe1_R");

	FVector Location = Character->GetMesh()->GetBoneLocation(BoneName);

	TWeakObjectPtr<UBaseCharacterAttackComponent> WeakThis = this;

	TraceChannelHelper::SphereSingleByChannel
	(
		GetWorld(),
		Character,
		Location,
		Location,
		FRotator::ZeroRotator,
		ECC_Camera,
		25.f,
		true,
		true,
		[WeakThis] (bool bHit, FHitResult HitResult)
		{
			if (WeakThis.IsValid())
			{
				WeakThis->Multicast_OnHitTraceChannel_Implementation(bHit, HitResult, 10);
			}
		}
	);
	
}

void UBaseCharacterAttackComponent::OnHeadButtTraceChannel()
{
	Server_OnHeadButtTraceChannel();
}


void UBaseCharacterAttackComponent::Server_OnHeadButtTraceChannel_Implementation()
{
	FName BoneName = TEXT("Head");

	FVector Location = Character->GetMesh()->GetBoneLocation(BoneName);

	TWeakObjectPtr<UBaseCharacterAttackComponent> WeakThis = this;

	TraceChannelHelper::SphereSingleByChannel
	(
		GetWorld(),
		Character,
		Location,
		Location,
		FRotator::ZeroRotator,
		ECC_Camera,
		50.f,
		true,
		true,
		[WeakThis] (bool bHit, FHitResult HitResult)
		{
			if (WeakThis.IsValid())
			{
				WeakThis->Multicast_OnHitTraceChannel_Implementation(bHit, HitResult, 10);
			}
		}
	);
}

void UBaseCharacterAttackComponent::Multicast_OnHitTraceChannel_Implementation(bool bHit, FHitResult HitResult, float damage)
{
	if (bHit)
	{
		AActor* HitActor = HitResult.GetActor();
		if (AlreadyHitActorsDuringAttack.Contains(HitActor))
		{
			return;
		}

		AlreadyHitActorsDuringAttack.Add(HitActor);
		
		if (ABaseCharacter* Other = Cast<ABaseCharacter>(HitActor))
		{
			Other->OnHit(HitResult.ImpactNormal, damage);
		}
	}
}
