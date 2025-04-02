#include "Character/BaseCharacterAttackComponent.h"

#include "EnhancedInputComponent.h"
#include "KismetTraceUtils.h"
#include "Character/BaseCharacter.h"
#include "Character/BaseCharacterAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"
#include "Weapon/Weapon.h"

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
	DOREPLIFETIME(UBaseCharacterAttackComponent, AlreadyHitActorsDuringAttack);
	DOREPLIFETIME(UBaseCharacterAttackComponent, bIsAttacking);
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

	if (Character->CheckAndStopKnockdown())
	{
		return;
	}

	PunchPressingTime = 0;
	bBeginPunchInput = true;
}

void UBaseCharacterAttackComponent::Punch()
{
	if (Character->CheckAndStopKnockdown())
	{
		return;
	}
	
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

	if (Character->bHasWeapon)
	{
		if (Character->OwnedWeapon)
		{
			const UEnum* EnumPtr = StaticEnum<EWeaponType>();
			FString SelectionName = EnumPtr->GetDisplayNameTextByValue(static_cast<uint8>(Character->OwnedWeapon->GetWeaponType())).ToString();
			PlayWeaponAttackAnimMontage(SelectionName);
		}
		return;
	}
	
	FString BoneName = ArmDirection == EArmDirection::LEFT? TEXT("UpperArm_L") : TEXT("UpperArm_R");

	if (!Character->IsLocallyControlled())
	{
		return;
	}
	
	Server_PlayAnimMontage(PunchAnimMontage, 1.f, *BoneName);

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

	if (Character->CheckAndStopKnockdown())
	{
		return;
	}

	bIsAttacking = true;

	if (!Character->IsLocallyControlled())
	{
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

		if (!Character->IsLocallyControlled())
		{
			return;
		}
		
		Server_PlayAnimMontage(KickAnimMontage, 1.5f);
	}
	
}

void UBaseCharacterAttackComponent::Finish()
{
	bIsAttacking = false;

	AlreadyHitActorsDuringAttack.Empty();
	if (Character && Character->bHasWeapon && Character->OwnedWeapon)
	{
		Character->OwnedWeapon->FinishAttack();
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
	// FString BoneName = ArmDirection == EArmDirection::LEFT? TEXT("UpperArm_L") : TEXT("UpperArm_R");
	//
	// FVector ForceDirection = Character->GetActorForwardVector() * ForceAmount;
	// Character->GetMesh()->AddImpulseToAllBodiesBelow(ForceDirection, *BoneName, true);
}

void UBaseCharacterAttackComponent::OnPunchTraceChannel()
{
	FName BoneName = ArmDirection == EArmDirection::LEFT? TEXT("Hand_R") : TEXT("Hand_L");

	if (!Character->IsLocallyControlled())
	{
		return;
	}

	Server_OnHitTraceChannel(EAttackType::PUNCH, BoneName, 20.f, PunchDamage);
}

void UBaseCharacterAttackComponent::OnKickTraceChannel()
{
	if (!Character->IsLocallyControlled())
	{
		return;
	}
	
	Server_OnHitTraceChannel(EAttackType::KICK, TEXT("FootToe1_R"), 25.f, KickDamage);
}

void UBaseCharacterAttackComponent::OnHeadButtTraceChannel()
{
	if (!Character->IsLocallyControlled())
	{
		return;
	}
	
	Server_OnHitTraceChannel(EAttackType::HEAD_BUTT, TEXT("Head"), 50.f, HeadButtDamage);
}

void UBaseCharacterAttackComponent::PlayWeaponAttackAnimMontage(FString SelectionName)
{
	if (!Character->IsLocallyControlled())
	{
		return;
	}

	Server_PlayWeaponAttackAnimMontage(SelectionName);
}

void UBaseCharacterAttackComponent::Server_PlayWeaponAttackAnimMontage_Implementation(const FString& SelectionName)
{
	Multicast_PlayWeaponAttackAnimMontage(SelectionName);
}

void UBaseCharacterAttackComponent::Multicast_PlayWeaponAttackAnimMontage_Implementation(const FString& SelectionName)
{
	Character->PlayAnimMontage(WeaponAttackAnimMontage, 1.f, *SelectionName);
}

void UBaseCharacterAttackComponent::Server_OnHitTraceChannel_Implementation(EAttackType Type, FName BoneName, float Radius, float Damage)
{
	FVector Location = Character->GetMesh()->GetBoneLocation(BoneName);

	TArray<FHitResult> HitResults;
	FCollisionQueryParams CollisionParams;

	CollisionParams.AddIgnoredActor(Character);
	CollisionParams.AddIgnoredComponent(Character->GetMesh());
	
	bool bHit = GetWorld()->SweepMultiByChannel(
		HitResults,
		Location,
		Location,
		FQuat::Identity,
		ECC_Camera,
		FCollisionShape::MakeSphere(Radius),
		CollisionParams
	);

	if (DrawDebug)
	{
		// DrawDebugSphereTraceMulti(
		// 	GetWorld(),
		// 	Location,
		// 	Location,
		// 	Radius,
		// 	EDrawDebugTrace::ForDuration,
		// 	bHit,
		// 	HitResults,
		// 	FColor::Yellow,
		// 	FColor::Green,
		// 	1.f
		// );
	}

	if (bHit)
	{
		Multicast_OnHitTraceChannel_Implementation(Type, bHit, HitResults, Damage);
	}
}

void UBaseCharacterAttackComponent::Multicast_OnHitTraceChannel_Implementation(EAttackType Type, bool bHit, const TArray<FHitResult>& HitResults, float Damage)
{
	if (bHit)
	{
		for (FHitResult HitResult : HitResults)
		{
			AActor* HitActor = HitResult.GetActor();
			if (AlreadyHitActorsDuringAttack.Contains(HitActor))
			{
				continue;
			}

			AlreadyHitActorsDuringAttack.Add(HitActor);
		
			if (ABaseCharacter* Other = Cast<ABaseCharacter>(HitActor))
			{
				Other->OnHit(Type, HitResult.Normal.GetSafeNormal(), Damage);
			}
		}
	}
}