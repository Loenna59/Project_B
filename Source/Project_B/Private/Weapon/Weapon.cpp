#include "Weapon/Weapon.h"

#include "Components/CapsuleComponent.h"

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

void AWeapon::SetVisible(bool bVisible)
{
	SetHidden(!bVisible);
}

void AWeapon::FinishAttack()
{
	AlreadyHitActorsDuringAttack.Empty();
}

