// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "Hammer.generated.h"

UCLASS()
class PROJECT_B_API AHammer : public AWeapon
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AHammer();

protected:
	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere)
	class UCapsuleComponent* Trigger;

	UPROPERTY(VisibleAnywhere)
	class USceneComponent* HitPoint;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bDrawDebug = false;

	virtual void ToggleSimulatePhysics(bool bSimulate) override;

	virtual EWeaponType GetWeaponType() const override { return EWeaponType::TwoHanded; }

	virtual void OnAttackTraceChannel() override;

	virtual void SetVisible(bool bVisible) override;
};
