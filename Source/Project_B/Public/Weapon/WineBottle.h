// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/Weapon.h"
#include "WineBottle.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_B_API AWineBottle : public AWeapon
{
	GENERATED_BODY()

public:
	AWineBottle();

protected:
	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* Mesh;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bDrawDebug = false;

	virtual void ToggleSimulatePhysics(bool bSimulate) override;

	virtual EWeaponType GetWeaponType() const override { return EWeaponType::OneHanded; }

	virtual void OnAttackTraceChannel() override;
	
};
