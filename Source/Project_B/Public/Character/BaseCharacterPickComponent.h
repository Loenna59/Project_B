// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacterInputComponent.h"
#include "BaseCharacterPickComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnGrabbing);
DECLARE_MULTICAST_DELEGATE(FOnRelease);


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

protected:
	UPROPERTY()
	class UInputAction* PickInputAction;

public:
	FOnGrabbing OnGrabbing;

	FOnRelease OnRelease;
};
