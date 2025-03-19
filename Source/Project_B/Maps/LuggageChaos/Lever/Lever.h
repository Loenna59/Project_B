// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Lever.generated.h"

class USphereComponent;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLeverValueChanged, float, NewValue);

class ABaseLeverInteractor;

UCLASS()
class PROJECT_B_API ALever : public AActor
{
	GENERATED_BODY()
	
public:
	ALever();
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UStaticMeshComponent* LeverMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	USceneComponent* LeverPivot;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	class UPhysicsConstraintComponent* LeverConstraint;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USphereComponent* SphereCollision;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Interactor)
	//ALeverInteractor* InteractorObject;
	
private:
	UPROPERTY()
	USceneComponent* Root;
	
	float LeverValue = 0.0f;

	bool bIsInteracting = false;

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	void SetLeverValue(float value);
	float GetLeverValue() const { return LeverValue;}

	FTimerHandle DecreaseValueTimerHandle;

	void StartDecreaseValue();   
	void DecreaseValueStep();   

	//델리게이트
	UPROPERTY()
	FOnLeverValueChanged OnLeverValueChanged;

private:	
	UFUNCTION()
	void OnCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	UFUNCTION()
	void OnCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
};
