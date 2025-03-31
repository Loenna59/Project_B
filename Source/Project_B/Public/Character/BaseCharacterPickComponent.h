// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacterInputComponent.h"
#include "BaseCharacterPickComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnGrabbing);
DECLARE_MULTICAST_DELEGATE(FOnRelease);
DECLARE_MULTICAST_DELEGATE_OneParam(FOffPhysics, EGrabState);

UENUM(BlueprintType, meta = (Bitflags))
enum class EGrabState : uint8
{
	None = 0 UMETA(DisplayName = "None"),
	Left = 1 << 0 UMETA(DisplayName = "Left"),
	Right = 1 << 1 UMETA(DisplayName = "Right"),
	Both = Left | Right UMETA(DisplayName = "Both")
};

ENUM_CLASS_FLAGS(EGrabState);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECT_B_API UBaseCharacterPickComponent : public UBaseCharacterInputComponent
{
	GENERATED_BODY()

public:
	UBaseCharacterPickComponent();

protected:
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

public:
	virtual void SetupInputBiding(class UEnhancedInputComponent* input) override;

	UFUNCTION()
	void Picking();

	UFUNCTION()
	void ReleasePick();

	UFUNCTION()
	void UpdateGrabState(EGrabState State, bool bIsGrabbing);

	UFUNCTION()
	void ValidateGrabState();

	// Getter 만들게요 언니...
	const EGrabState& GetGrabState() const {return GrabState;}

protected:
	UPROPERTY()
	class UInputAction* PickInputAction;

	UPROPERTY()
	EGrabState GrabState = EGrabState::None;

	FTimerHandle ValidTimerHandle;

public:
	FOnGrabbing OnGrabbing;

	FOnRelease OnRelease;

	FOffPhysics OffPhysics;

	UPROPERTY()
	EGrabState BlockGrabState = EGrabState::None;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ValidationThreshold = 1.f;
};
