// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "BaseCharacterPhysicsAnimComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECT_B_API UBaseCharacterPhysicsAnimComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UBaseCharacterPhysicsAnimComponent();

protected:
	virtual void InitializeComponent() override;
	
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	void TogglePhysicalAnimation(bool toggle);

	UFUNCTION(Server, Reliable)
	void Server_TogglePhysicalAnimation(FName BoneName, bool bSimulate);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_TogglePhysicalAnimation(FName BoneName, bool bSimulate);

	UFUNCTION()
	void TogglePhysicalAnimationInternal(FName BoneName, bool bSimulate);

protected:
	UPROPERTY()
	class ABaseCharacter* Character = nullptr;

	UPROPERTY()
	class UBaseCharacterAnimInstance* AnimInstance = nullptr;

	UPROPERTY()
	class USkeletalMeshComponent* Mesh = nullptr;
	
	UPROPERTY()
	class UPhysicalAnimationComponent* PhysicalAnimationComp = nullptr;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	FName SimulateBoneName = TEXT("CharacterPelvis");

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ForwardForceAmount = 1000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bAwakePhysics = true;
};
