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
	
	UPROPERTY(VisibleAnywhere)
	class UBaseCharacterMoveComponent* MoveComp;

	UPROPERTY(VisibleAnywhere)
	class UBaseCharacterPhysicsAnimComponent* PhysicsAnimComp;

};
