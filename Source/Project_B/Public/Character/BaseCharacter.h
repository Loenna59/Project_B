#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Character/AttackType.h"
#include "BaseCharacter.generated.h"

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
	void Server_OnPlayHitMontage(EAttackType Type, float ForwardDot, float SideDot);
	
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_OnPlayHitMontage(EAttackType Type, float ForwardDot, float SideDot);

	UFUNCTION()
	void TakeWeapon(class AWeapon* Weapon);

	UFUNCTION(Server, Reliable)
	void Server_TakeWeapon(class AWeapon* Weapon);

	UFUNCTION()
	void AttachWeapon();

protected:
	UPROPERTY()
	class UInputMappingContext* IMC = nullptr;

	float CurrentHealth;

public:
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxHealth = 100;

	UPROPERTY()
	bool IsDead = false;

	UPROPERTY()
	bool bHasWeapon = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UAnimMontage* KnockdownMontage;

	UPROPERTY(ReplicatedUsing=AttachWeapon)
	class AWeapon* OwnedWeapon = nullptr;
	
};
