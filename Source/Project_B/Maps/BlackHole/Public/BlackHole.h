
#pragma once

#include "CoreMinimal.h"
#include "Character/BaseCharacter.h"
#include "GameFramework/Actor.h"
#include "BlackHole.generated.h"

UCLASS()
class PROJECT_B_API ABlackHole : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABlackHole();

protected:
	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	               const FHitResult& SweepResult);
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// 스폰되었니? 변수
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsActive = false;
	// 중력필드를 한번만 생성하기 위한 변수
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bGravityFieldCreated = false;

	// 블랙홀 힘
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float OrbitPower = 500.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float OrbitScale = 1.0f;
	bool bOrbitalVelocityApplied = false;

	// 외관
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USceneComponent* Root;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* Sphere;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USphereComponent* SphereComp;

	// 스폰(소멸)될 시간이 되면, 크기 조절

	// 체인
	void ActivateBlackhole();
	UPROPERTY()
	FVector GravityVel;
	
	// 공전 속도 값
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float OrbitSpeed = 30.0f;
	
};


