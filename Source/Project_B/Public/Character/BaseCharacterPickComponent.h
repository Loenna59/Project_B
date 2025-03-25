// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacterInputComponent.h"
#include "InputActionValue.h"
#include "BaseCharacterPickComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECT_B_API UBaseCharacterPickComponent : public UBaseCharacterInputComponent
{
	GENERATED_BODY()

public:
	UBaseCharacterPickComponent();

protected:
	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

public:
	virtual void SetupInputBiding(class UEnhancedInputComponent* input) override;

	UFUNCTION()
	void BeginPick();

	UFUNCTION()
	void Picking();

	UFUNCTION()
	void ReleasePick();

protected:
	UPROPERTY()
	class UInputAction* PickInputAction;

	UPROPERTY()
	class UBaseCharacterArmComponent* LeftArmComp;

	UPROPERTY()
	class UBaseCharacterArmComponent* RightArmComp;
};
