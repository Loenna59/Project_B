// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GravityComponent.generated.h"


class ABlackHole;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECT_B_API UGravityComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UGravityComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	void ApplyGravity(float DeltaTime);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gravity")
	AActor* Planet; // 중력을 적용할 행성 (외부에서 설정 가능)
	ABlackHole* Blackhole;

	UPROPERTY(EditAnywhere, Category = "Gravity")
	float GravityConstant = 9.8f; // 기본 중력 가속도
	UPROPERTY(EditAnywhere, Category = "Gravity")
	float PlanetMass = 10000.0f; // 행성의 질량
	UPROPERTY(EditAnywhere, Category = "Gravity")
	float StableOrbitRadius = 500.0f; // 목표 궤도 반지름
	UPROPERTY(EditAnywhere, Category = "Gravity")
	float RotationSpeed = 20.0f; // 자전 속도
	UPROPERTY(EditAnywhere, Category = "Gravity")
	float OrbitSpeed = 100.0f; // 공전 속도

	// 캐릭터라면, 힘을 더 주자
	UPROPERTY(EditAnywhere, Category = "Gravity")
	float CharacterPower = 1.0f; 

private:
	UPrimitiveComponent* OwnerPhysicsComp; // 현재 오브젝트의 물리 컴포넌트
};


