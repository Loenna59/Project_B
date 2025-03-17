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

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	UPROPERTY()
	class UInputAction* MoveInputAction = nullptr;

	UPROPERTY()
	class UInputAction* RotateInputAction = nullptr;

	UPROPERTY()
	class UInputAction* JumpInputAction = nullptr;

	UPROPERTY()
	class UInputAction* RunInputAction = nullptr;

	UPROPERTY(EditAnywhere, Category=Movement)
	float WalkSpeed = 500.f;

	UPROPERTY(EditAnywhere, Category=Movement)
	float RunSpeed = 1000.f;

	UPROPERTY(Replicated)
	float ReplicatedSpeed;

public:
	virtual void BeginPlay() override;

	virtual void SetupInputBiding(class UEnhancedInputComponent* input) override;

protected:
	UFUNCTION()
	void Move(const FInputActionValue& actionValue);

	UFUNCTION()
	void Rotate(const FInputActionValue& actionValue);

	UFUNCTION()
	void StartJump();

	UFUNCTION()
	void EndJump();

	UFUNCTION()
	void StartRun();

	UFUNCTION()
	void EndRun();

	UFUNCTION(Server, Reliable)
	void Server_UpdateSpeed(float Speed);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_UpdateSpeed(float Speed);

	void UpdateSpeed(float Speed);
};
