#include "Weapon/Weapon.h"

#include "Character/BaseCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Project_B/Utilities/LogMacro.h"

AWeapon::AWeapon()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		SetReplicates(true);
		SetReplicateMovement(true);
	}

	CurrentCapacity = MaxCapacity;
}

void AWeapon::DecreaseCapacity()
{
	CurrentCapacity--;
	// LOG_SCREEN("%d", CurrentCapacity);
	if (CurrentCapacity <= 0)
	{
		if (GetOwner() != nullptr)
		{
			if (ABaseCharacter* Character = Cast<ABaseCharacter>(GetOwner()))
			{
				Character->Unequip();
			}
		}

		SetVisible(false);
		bool _ = OnRequestRespawn.ExecuteIfBound(GetAttackType(), LocateIndex);
	}
}

void AWeapon::SetVisible(bool bVisible, int32 SpawnPointIndex)
{
	GetWorld()->GetTimerManager().ClearTimer(DisappearTimerHandle);
	
	SetHidden(!bVisible);
	
	if (bVisible && GetOwner() == nullptr)
	{
		if (SpawnPointIndex < 0)
		{
			return;
		}
		
		TWeakObjectPtr<AWeapon> WeakThis = this;
		LocateIndex = SpawnPointIndex;
		
		GetWorld()->GetTimerManager().SetTimer(
			DisappearTimerHandle,
			[WeakThis, SpawnPointIndex]()
			{
				if (WeakThis.IsValid())
				{
					WeakThis->SetVisible(false);
					bool _ = WeakThis->OnRequestRespawn.ExecuteIfBound(WeakThis->GetAttackType(), SpawnPointIndex);
				}
			},
			DisappearTime,
			false
		);
	}
	else
	{
		CurrentCapacity = MaxCapacity;
	}
}

void AWeapon::FinishAttack()
{
	AlreadyHitActorsDuringAttack.Empty();
}

void AWeapon::CancelDisappear()
{
	GetWorld()->GetTimerManager().ClearTimer(DisappearTimerHandle);
}

void AWeapon::RestartDisappear()
{
	TWeakObjectPtr<AWeapon> WeakThis = this;
		
	GetWorld()->GetTimerManager().SetTimer(
		DisappearTimerHandle,
		[WeakThis]()
		{
			if (WeakThis.IsValid())
			{
				bool _ = WeakThis->OnRequestRespawn.ExecuteIfBound(WeakThis->GetAttackType(), WeakThis->LocateIndex);
			}
		},
		5.f,
		false
	);
}

