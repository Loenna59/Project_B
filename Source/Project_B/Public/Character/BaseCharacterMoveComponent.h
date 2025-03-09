// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacterInputComponent.h"
#include "InputActionValue.h"
#include "Components/ActorComponent.h"
#include "BaseCharacterMoveComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECT_B_API UBaseCharacterMoveComponent : public UBaseCharacterInputComponent
{
	GENERATED_BODY()

public:
	UBaseCharacterMoveComponent();

protected:
	UPROPERTY()
	class UInputAction* MoveInputAction = nullptr;

	UPROPERTY()
	class UInputAction* RotateInputAction = nullptr;

	UPROPERTY()
	class UInputAction* JumpInputAction = nullptr;
	
public:
	virtual void SetupInputBiding(class UEnhancedInputComponent* input) override;

protected:
	UFUNCTION()
	void Move(const FInputActionValue& actionValue);
};
