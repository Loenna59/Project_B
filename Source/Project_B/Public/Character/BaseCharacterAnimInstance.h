// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacterPickComponent.h"
#include "Animation/AnimInstance.h"
#include "Character/AttackType.h"
#include "BaseCharacterAnimInstance.generated.h"

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	None UMETA(DisplayName = "None"),
	OneHanded UMETA(DisplayName = "OneHanded"),
	TwoHanded UMETA(DisplayName = "TwoHanded"),
	Ranged UMETA(DisplayName = "Ranged")
};

UCLASS()
class PROJECT_B_API UBaseCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
protected:
	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class ABaseCharacter* Character;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UCharacterMovementComponent* MovementComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FVector Velocity;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float GroundSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bShouldMove;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsFalling;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsHit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D HitDirection;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<EGrabState, FVector> HandIKTarget = { { EGrabState::Right, FVector::ZeroVector }, { EGrabState::Left, FVector::ZeroVector } };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<EGrabState, float> HandIKAlpha = { { EGrabState::Right, 0 }, { EGrabState::Left, 0 } };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<EGrabState, bool> bIsGrabbing = { { EGrabState::Right, false }, { EGrabState::Left, false } };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<EGrabState, FVector> HandIKJoint = { { EGrabState::Right, FVector::ZeroVector }, { EGrabState::Left, FVector::ZeroVector } };
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EWeaponType CurrentWeaponType = EWeaponType::None;

	UFUNCTION(BlueprintImplementableEvent)
	void StartHitProcess(float ForwardDot, float SideDot);
};
