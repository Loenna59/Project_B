// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacterInputComponent.h"
#include "Character/AttackType.h"
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

	UPROPERTY()
	bool bBeginPunchInput = false;

	UPROPERTY()
	float PunchPressingTime = 0;

	const float PunchExecuteThreshold = 0.5f;

	UPROPERTY(Replicated)
	TArray<AActor*> AlreadyHitActorsDuringAttack;
	

public:
	UPROPERTY(EditAnywhere, Replicated, Category=Attack)
	class UAnimMontage* PunchAnimMontage;

	UPROPERTY(EditAnywhere, Replicated, Category=Attack)
	class UAnimMontage* HeadButtAnimMontage;
	
	UPROPERTY(EditAnywhere, Replicated, Category=Attack)
	class UAnimMontage* KickAnimMontage;

	UPROPERTY()
	bool bIsAttacking = false;

	UPROPERTY(EditAnywhere, Category=Attack)
	float PunchDamage = 0;

	UPROPERTY(EditAnywhere, Category=Attack)
	float KickDamage = 0;

	UPROPERTY(EditAnywhere, Category=Attack)
	float HeadButtDamage = 0;

	UPROPERTY(EditAnywhere)
	bool DrawDebug = true;
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

public:
	virtual void SetupInputBiding(class UEnhancedInputComponent* input) override;

	UFUNCTION()
	void BeginPunch();

	UFUNCTION()
	void Punch();

	UFUNCTION()
	void HeadButt();

	UFUNCTION()
	void Kick();

	UFUNCTION()
	void Finish();

	UFUNCTION(Server, Reliable)
	void Server_PlayAnimMontage(UAnimMontage* Montage, float PlayRate = 1.f, FName SectionName = NAME_None);
	
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_PlayAnimMontage(UAnimMontage* Montage, float PlayRate = 1.f, FName SectionName = NAME_None);

	UFUNCTION()
	void AddForceForwardVector();

	UFUNCTION()
	void OnPunchTraceChannel();

	UFUNCTION()
	void OnKickTraceChannel();

	UFUNCTION()
	void OnHeadButtTraceChannel();

	UFUNCTION(Server, Reliable)
	void Server_OnHitTraceChannel(EAttackType Type, FName BoneName, float Radius, float Damage);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_OnHitTraceChannel(EAttackType Type, bool bHit, FHitResult HitResult, float Damage);

};
