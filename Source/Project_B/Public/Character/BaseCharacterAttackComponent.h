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

	UPROPERTY(Replicated)
	EArmDirection ArmDirection;

	UPROPERTY(EditAnywhere, Category=Attack)
	float ForceAmount = 1000.f;

	UPROPERTY(EditAnywhere, Category=Attack)
	float DropkickForceAmount = 10000.f;

public:
	UPROPERTY(EditAnywhere, Replicated, Category=Attack)
	class UAnimMontage* PunchAnimMontage;

	UPROPERTY(EditAnywhere, Replicated, Category=Attack)
	class UAnimMontage* HeadButtAnimMontage;
	
	UPROPERTY(EditAnywhere, Replicated, Category=Attack)
	class UAnimMontage* KickAnimMontage;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

public:
	virtual void SetupInputBiding(class UEnhancedInputComponent* input) override;

	UFUNCTION()
	void Punch();

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_Punch(UAnimMontage* Montage, EArmDirection Direction);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_Punch(UAnimMontage* Montage, EArmDirection Direction);
	

	UFUNCTION()
	void HeadButt();

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_HeadButt(UAnimMontage* Montage);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_HeadButt(UAnimMontage* Montage);
	

	UFUNCTION()
	void Kick();

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_Kick(UAnimMontage* Montage);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_Kick(UAnimMontage* Montage);
	
	
	void AddForceForwardVector();
};
