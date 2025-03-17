
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BlackHole.generated.h"

// 블랙홀이 활성화 되었는지 판단하는 델리게이트
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBlackholeStateChanged, bool, bNewState);

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float OrbitPower = 10000.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float OrbitScale = 1.0f;

	// 블랙홀 활성화 델리게이트?
	void SetBlackholeState(bool bNewState);
	UPROPERTY(BlueprintAssignable)
	FOnBlackholeStateChanged OnBlackholeStateChanged;

	// 외관
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USceneComponent* Root;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* Sphere;
	UPROPERTY(editAnywhere, BlueprintReadWrite)
	class USphereComponent* FirstR;
	// 중력 필드 (ON/OFF)
	UPROPERTY(editAnywhere, BlueprintReadWrite)
	class URadialForceComponent* GravityField;

	// 스폰(소멸)될 시간이 되면, 크기 조절

	// 빨아들일 요소들을 조사하고 가동 & 멈춤
	void ActivateBlackhole();
	void DeactivateBlackhole();
	// 중력필드를 형성하고 회전시킴
	void CreateGravityField();
	void ApplyOrbitalForce();
	
	// 소환 횟수 카운트
	int32 SpawnCount;
	
};


