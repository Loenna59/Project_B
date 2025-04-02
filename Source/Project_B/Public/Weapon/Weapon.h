#pragma once

#include "CoreMinimal.h"
#include "Character/BaseCharacterAnimInstance.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

DECLARE_DELEGATE_TwoParams(FOnRequestRespawn, EAttackType, int32);

UCLASS()
class PROJECT_B_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	AWeapon();

protected:
	virtual void BeginPlay() override;

	UPROPERTY()
	TArray<AActor*> AlreadyHitActorsDuringAttack;

	int32 LocateIndex = -1;

public:
	UPROPERTY(EditAnywhere)
	int32 MaxCapacity = 10;

	int32 CurrentCapacity;

	float DisappearTime = 30.f; 

	virtual void ToggleSimulatePhysics(bool bSimulate) {}

	virtual EWeaponType GetWeaponType() const { return EWeaponType::None; }

	virtual EAttackType GetAttackType() const { return EAttackType::PUNCH; }

	virtual void OnAttackTraceChannel() {}

	void DecreaseCapacity();

	virtual void SetVisible(bool bVisible, int32 SpawnPointIndex = -1);

	void FinishAttack();

	void CancelDisappear();

	void RestartDisappear();

	FTimerHandle DisappearTimerHandle;

	FOnRequestRespawn OnRequestRespawn;
};
