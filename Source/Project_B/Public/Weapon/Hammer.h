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
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere)
	class UCapsuleComponent* Trigger;

	UPROPERTY(VisibleAnywhere)
	class USceneComponent* HitPoint;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void ToggleSimulatePhysics(bool bSimulate) override;

	virtual EWeaponType GetWeaponType() const override { return EWeaponType::TwoHanded; }
};
