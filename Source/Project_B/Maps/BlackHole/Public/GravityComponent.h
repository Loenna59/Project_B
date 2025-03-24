// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BlackholeGameMode.h"
#include "BlackholeGameState.h"
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
	virtual void InitializeComponent() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	// 블랙홀을 적용하자
	void ApplyGravity(float DeltaTime);
	// 동기화 함수
	// *물리는 서버에서만 처리되기때문에 물리를 이용한 변경 코드는 서버에서만 실행하는 것이 좋다
	UFUNCTION(NetMulticast, reliable)
	void MulticastRPC_SetRotation(FVector AngularVelocity);	// 자전
	UFUNCTION(NetMulticast, reliable)
	void MulticastRPC_SetGravity(FVector GravityStrength); // 중력값
	UFUNCTION(NetMulticast, reliable)
	void MulticastRPC_SetOrbit(FVector AngularVelocity); // 궤도 회전값

	// 블랙홀 비활성화
	void DeactivateGravity();

	// 플레이어 물리 활성화 변수 동기화
	UPROPERTY(ReplicatedUsing=OnRep_SimulatingPhysics)
	bool bPhysics = false;
	UFUNCTION()
	void OnRep_SimulatingPhysics();
	
	// 플레이어 캡슐 정렬
	FTimerHandle AlignHandle;
	void AlignCharacter();

	// 카운트
	void SpawnCount();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gravity")
	AActor* Planet; // 중력을 적용할 행성 (외부에서 설정 가능)
	ABlackHole* Blackhole;
	ABlackholeGameState* gs;
	ACharacter* PlayerCharacter;

	// 페이즈별 적절하게 조절 필요, 동기화 필요
	// 목표 궤도 반지름
	UPROPERTY(Replicated)
	float OrbitRadius = 1500.0f;
	// 자전 속도
	UPROPERTY(Replicated)
	float RotationSpeed = 20.0f;
	// 공전 속도
	UPROPERTY(Replicated)
	float OrbitSpeed = 50.0f;
	// 선형 보간 값
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gravity")
	float InterpSpeed;

	// 현재 궤도 각도 설정 (클라이언트에서도 변경 적용)
	float CurrentOrbitAngle = 0.0f;
	
	// 공용 변수들
	FVector PlanetLocation;
	FVector ObjectLocation;
	FVector Direction; // 오브젝트->블랙홀을 바라보는 방향
	float Distance; // 둘 사이 거리

private:
	UPrimitiveComponent* OwnerPhysicsComp; // 현재 오브젝트의 물리 컴포넌트
};


