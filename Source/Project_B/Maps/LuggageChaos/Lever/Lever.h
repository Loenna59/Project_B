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

	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* Trigger;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ToZeroSpeed = 1.5f;
	
private:
	UPROPERTY()
	USceneComponent* Root;

	UPROPERTY(Replicated)
	float LeverValue = 0.0f;

	bool bIsInteracting = false;

protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
public:
	virtual void Tick(float DeltaTime) override;

	void SetLeverValue(float value);
	float GetLeverValue() const { return LeverValue;}

	FTimerHandle DecreaseValueTimerHandle;

	//델리게이트
	UPROPERTY()
	FOnLeverValueChanged OnLeverValueChanged;

private:	
	UFUNCTION()
	void OnCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	UFUNCTION()
	void OnCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(Server, Reliable)
	void Server_IsInteracting(bool isInteract);
	UFUNCTION(NetMulticast, Reliable)
	void Net_IsInteracting(bool isInteract);
	
	void LeverInteracting(float dt);

	// LeverValue 천천히 0으로 줄이는 함수
	void StartDecreaseValue();   
	void DecreaseValueStep();   
};
