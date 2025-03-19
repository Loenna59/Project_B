
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

	// 중력 필드 (ON/OFF)
	UPROPERTY(editAnywhere, BlueprintReadWrite)
	class URadialForceComponent* GravityField;

	// 스폰(소멸)될 시간이 되면, 크기 조절

	// 빨아들일 요소들을 조사하고 가동 / 소멸
	void ActivateBlackhole();
	void DeactiveBlackhole();

	// 요소
	ABaseCharacter* Player;
	
	// 중력필드를 형성하고, 액터들 회전시킴
	void CreateGravityField();
	void ActivateGravity();
	void ActivateOrbital();
	
	// 공전궤도 값
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float R = 0.f;
	float CurrentAngle = 0.f;
	// 공전 속도 값
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float OrbitSpeed = 30.0f;
};


