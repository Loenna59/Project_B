// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacterPhysicsAnimComponent.h"
#include "BaseCharacterPickComponent.h"
#include "BaseCharacterArmComponent.generated.h"

DECLARE_DELEGATE_TwoParams(FOnUpdateGrabState, EGrabState, bool);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECT_B_API UBaseCharacterArmComponent : public UBaseCharacterPhysicsAnimComponent
{
	GENERATED_BODY()

public:
	UBaseCharacterArmComponent();

protected:
	virtual void BeginPlay() override;

	UPROPERTY()
	class UBaseCharacterPickComponent* PickComp = nullptr;

	UPROPERTY()
	class UPrimitiveComponent* GrabComponent = nullptr;

	UPROPERTY()
	class UPhysicsConstraintComponent* GrabConstraintComp = nullptr;
	
public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	void Grabbing();

	UFUNCTION()
	void ReleaseGrab();

	UFUNCTION()
	void DetectNearby(bool bHit, TArray<FHitResult> HitResults);

	UFUNCTION()
	void CheckAndOffPhysics(EGrabState BlockState);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EGrabState GrabState = EGrabState::None;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Radius = 20;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float GrabTraceDistance = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float JointOffsetAmount = 20.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName SocketName = TEXT("Hand");

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName BoneName = TEXT("Hand_R");

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName JointBoneName = TEXT("UpperArm");

	UPROPERTY()
	class AActor* GrabbedActor;

	UPROPERTY()
	class UPhysicsHandleComponent* PhysicsHandleComp; // 붙잡은 액터와 연결해주는 handle

	FOnUpdateGrabState OnUpdateGrabState;
};
