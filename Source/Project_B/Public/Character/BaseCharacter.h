#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Character/AttackType.h"
#include "BaseCharacter.generated.h"

DECLARE_DELEGATE_OneParam(FOnCalculateSpeedByMass, float);

UCLASS()
class PROJECT_B_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ABaseCharacter();

protected:
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void OnHit(EAttackType Type, FVector NormalPoint, float damage);

	UFUNCTION(Server, Reliable)
	void Server_OnPlayHitMontage(EAttackType Type, FVector NormalPoint);
	
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_OnPlayHitMontage(EAttackType Type, float ForwardDot, float SideDot, FVector LaunchVelocity);

	UFUNCTION(Client, Reliable)
	void Client_SetEnableInput(bool bEnable);

	UFUNCTION()
	void TakeWeapon(class AWeapon* Weapon);

	UFUNCTION(Server, Reliable)
	void Server_TakeWeapon(class AWeapon* Weapon);

	UFUNCTION()
	void AttachWeapon();

	UFUNCTION()
	void OnWeaponAttackTraceChannel();

	UFUNCTION()
	void Unequip();

	UFUNCTION(Server, Reliable)
	void Server_UnequipWeapon();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_UnequipWeapon(AWeapon* Weapon);

	UFUNCTION()
	void CalculateSpeedByMass(float Mass);

	UFUNCTION()
	bool CheckAndStopKnockdown();

	UFUNCTION(Server, Reliable)
	void Server_CheckAndStopKnockdown();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_CheckAndStopKnockdown();
	
protected:
	UPROPERTY()
	class UInputMappingContext* IMC = nullptr;

	UPROPERTY()
	class UInputAction* InputActionUnequip = nullptr;

	float CurrentHealth;

public:
	UPROPERTY()
	class UEnhancedInputLocalPlayerSubsystem* InputSubsystem = nullptr;
	
	UPROPERTY()
	class UBaseCharacterAnimInstance* AnimInstance = nullptr;
	
	UPROPERTY(VisibleAnywhere)
	class USpringArmComponent* SpringArmComp = nullptr;

	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* CameraComp = nullptr;

	UPROPERTY()
	class UPhysicalAnimationComponent* PhysicalAnimationComp = nullptr;
	
	UPROPERTY(VisibleAnywhere)
	class UBaseCharacterMoveComponent* MoveComp = nullptr;

	UPROPERTY(VisibleAnywhere)
	class UBaseCharacterPhysicsAnimComponent* HeadPhysicsAnimComp = nullptr;

	UPROPERTY(VisibleAnywhere)
	class UBaseCharacterArmComponent* LeftArmPhysicsAnimComp = nullptr;

	UPROPERTY(VisibleAnywhere)
	class UBaseCharacterArmComponent* RightArmPhysicsAnimComp = nullptr;

	UPROPERTY(VisibleAnywhere)
	class UBaseCharacterPhysicsAnimComponent* RightFootPhysicsAnimComp = nullptr;

	UPROPERTY(VisibleAnywhere)
	class UBaseCharacterAttackComponent* AttackComp = nullptr;

	UPROPERTY(VisibleAnywhere)
	class UBaseCharacterPickComponent* PickComp = nullptr;

	UPROPERTY(VisibleAnywhere)
	class UGravityComponent* GravityComp = nullptr;

	UPROPERTY(VisibleAnywhere)
	class USceneComponent* TwoHandedSocket = nullptr;

	UPROPERTY(VisibleAnywhere)
	class USceneComponent* OneHandedSocket = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxHealth = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UAnimMontage* GetupAnimMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float KnockdownTime = 5.f;

	UPROPERTY()
	bool IsDead = false;

	UPROPERTY()
	bool bHasWeapon = false;

	UPROPERTY(Replicated)
	bool bIsKnockdown = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UAnimMontage* KnockdownMontage;

	UPROPERTY(ReplicatedUsing=AttachWeapon)
	class AWeapon* OwnedWeapon = nullptr;

	FOnCalculateSpeedByMass OnCalculateSpeedByMass;

	FTimerHandle KnockdownTimerHandle;
};
