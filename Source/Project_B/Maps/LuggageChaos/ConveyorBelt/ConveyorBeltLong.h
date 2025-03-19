// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ConveyorBeltLong.generated.h"

UCLASS()
class PROJECT_B_API AConveyorBeltLong : public AActor
{
	GENERATED_BODY()

public:
	AConveyorBeltLong();

	UPROPERTY(EditAnywhere)
	class UBoxComponent* EndBox;

	UPROPERTY(EditAnywhere)
	class UArrowComponent* StartArrow;
	
	UPROPERTY(EditAnywhere, Category = "Conveyor Belt")
	TArray<UStaticMeshComponent*> Plates;

	UPROPERTY(EditAnywhere, Category = "Conveyor Belt")
	uint8 PlateCount = 34;

	UPROPERTY(EditAnywhere, Category = "Conveyor Belt")
	float Speed = 100.0f;
	
	UPROPERTY(EditAnywhere, Category = "Conveyor Belt")
	UBoxComponent* ForceBox;

private:
	UPROPERTY(EditAnywhere)
	USceneComponent* Root;

	FVector MoveDir;

protected:
	virtual void BeginPlay() override;


public:
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION()
	void OnCharacterStepOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	UFUNCTION()
	void OnCharacterStepEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
