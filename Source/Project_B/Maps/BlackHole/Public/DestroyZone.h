// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DestroyZone.generated.h"

UCLASS()
class PROJECT_B_API ADestroyZone : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADestroyZone();

protected:
	UFUNCTION()
	void OnDestroyBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	                           bool bFromSweep, const FHitResult& SweepResult);
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// 외관 (회전체, 콜리전-겹치면 죽음)
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class USceneComponent* Root;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class UStaticMeshComponent* BottomBlade;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class UStaticMeshComponent* BottomRotator;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class USphereComponent* DestroyZoneSphere;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class URotatingMovementComponent* BladeRotateComp;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class URotatingMovementComponent* RotateComp;

	// 블랙홀이 가동되면, 움직이기 시작한다
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool bIsOnBlackhole = false;
	void BladeRotate();
	// 회전 속도값 변수
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float RotateSpeed = 180.f;
};
