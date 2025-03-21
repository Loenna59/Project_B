// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BlackholeGameMode.h"
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
	void DeactivateGravity();
	void SpawnCount();

	// 궤도 각도 설정
	float CurrentOrbitAngle = 0.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gravity")
	AActor* Planet; // 중력을 적용할 행성 (외부에서 설정 가능)
	ABlackHole* Blackhole;
	ABlackholeGameMode* gm;
	ACharacter* PlayerCharacter;

	// 플레이어 캡슐 정렬
	FTimerHandle AlignHandle;
	void AlignCharacter();

	// 페이즈별 적절하게 조절 필요
	// 목표 궤도 반지름
	UPROPERTY(EditAnywhere, Category = "Gravity")
	float OrbitRadius = 1500.0f;
	// 자전 속도
	UPROPERTY(EditAnywhere, Category = "Gravity")
	float RotationSpeed = 20.0f;
	// 공전 속도
	UPROPERTY(EditAnywhere, Category = "Gravity")
	float OrbitSpeed = 50.0f;
	// 선형 보간 값
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gravity")
	float InterpSpeed;

	// 공용 변수들
	FVector PlanetLocation;
	FVector ObjectLocation;
	// 오브젝트->블랙홀을 바라보는 방향
	FVector Direction;
	// 둘 사이 거리
	float Distance;

private:
	UPrimitiveComponent* OwnerPhysicsComp; // 현재 오브젝트의 물리 컴포넌트
};


