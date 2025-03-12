// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacterInputComponent.h"
#include "BaseCharacterAttackComponent.generated.h"

UENUM()
enum class EArmDirection : uint8
{
	LEFT,
	RIGHT
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECT_B_API UBaseCharacterAttackComponent : public UBaseCharacterInputComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UBaseCharacterAttackComponent();

protected:
	UPROPERTY()
	class UInputAction* PunchInputAction = nullptr;

	UPROPERTY()
	class UInputAction* HeadButtInputAction = nullptr;

	UPROPERTY()
	class UInputAction* KickInputAction = nullptr;

	UPROPERTY()
	EArmDirection ArmDirection;

	UPROPERTY(EditAnywhere, Category=Attack)
	float ForceAmount = 1000.f;

public:
	UPROPERTY(EditAnywhere, Category=Attack)
	class UAnimMontage* PunchAnimMontage;

	UPROPERTY(EditAnywhere, Category=Attack)
	class UAnimMontage* HeadButtAnimMontage;
	
	UPROPERTY(EditAnywhere, Category=Attack)
	class UAnimMontage* KickAnimMontage;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	virtual void SetupInputBiding(class UEnhancedInputComponent* input) override;

	UFUNCTION()
	void Punch();

	UFUNCTION()
	void HeadButt();

	UFUNCTION()
	void Kick();
	
	void AddForceForwardVector();
};
