#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

UCLASS()
class PROJECT_B_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ABaseCharacter();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	UPROPERTY()
	class UInputMappingContext* IMC;

public:
	UPROPERTY(VisibleAnywhere)
	class USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* CameraComp;

	UPROPERTY()
	class UPhysicalAnimationComponent* PhysicalAnimationComp;
	
	UPROPERTY(VisibleAnywhere)
	class UBaseCharacterMoveComponent* MoveComp;

	UPROPERTY(VisibleAnywhere)
	class UBaseCharacterPhysicsAnimComponent* HeadPhysicsAnimComp;

	UPROPERTY(VisibleAnywhere)
	class UBaseCharacterArmComponent* LeftArmPhysicsAnimComp;

	UPROPERTY(VisibleAnywhere)
	class UBaseCharacterArmComponent* RightArmPhysicsAnimComp;

	UPROPERTY(VisibleAnywhere)
	class UBaseCharacterPhysicsAnimComponent* RightFootPhysicsAnimComp;

	UPROPERTY(VisibleAnywhere)
	class UBaseCharacterAttackComponent* AttackComp;

	UPROPERTY(VisibleAnywhere)
	class UBaseCharacterPickComponent* PickComp;

	UPROPERTY(VisibleAnywhere)
	class UGravityComponent* GravityComp;

};
