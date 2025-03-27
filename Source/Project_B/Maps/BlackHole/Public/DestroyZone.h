// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DestroyZone.generated.h"

class ABlackHole;

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

	// 플레이어 상태 업데이트
	void UpdatePlayerState(const FString& playerKey);
	UFUNCTION(Server, Reliable)
	void ServerRPC_UpdatePlayerState(const FString& playerKey);
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_UpdatePlayerState(const FString& playerKey);

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
	ABlackHole* Blackhole;
	void Rotate(bool bIsRotate);
	
	// 회전 속도값 변수 (처음에는 0, 블랙홀 가동되면 180으로 바꿔주자)
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float RotateSpeed = 0.f;
};



