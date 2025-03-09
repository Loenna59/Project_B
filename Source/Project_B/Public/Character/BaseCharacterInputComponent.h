// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BaseCharacterInputComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECT_B_API UBaseCharacterInputComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UBaseCharacterInputComponent();

protected:
	virtual void BeginPlay() override;

	UPROPERTY()
	class ABaseCharacter* Character = nullptr;

	UPROPERTY()
	class UBaseCharacterAnimInstance* AnimInstance = nullptr;

public:
	virtual void SetupInputBiding(class UEnhancedInputComponent* input) {}
	
};
