#pragma once

#include "CoreMinimal.h"
#include "Character/BaseCharacterAnimInstance.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

UCLASS()
class PROJECT_B_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	AWeapon();

protected:
	virtual void BeginPlay() override;

public:	
	UPROPERTY(EditAnywhere)
	int32 MaxCapacity = 10;

	int32 CurrentCapacity;

	virtual void ToggleSimulatePhysics(bool bSimulate) {}

	virtual EWeaponType GetWeaponType() const { return EWeaponType::None; }

	virtual void OnAttackTraceChannel() {}

	virtual void SetVisible(bool bVisible);
};
