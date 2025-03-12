
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BlackHole.generated.h"

// 블랙홀이 활성화 되었는지 판단하는 변수
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

	// 스폰되었니?
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsActive = false;
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

	// 스폰(소멸)될 시간이 되면, 크기 조절

	// 빨아들일 요소들을 조사하고 가동, 멈춤
	void ActivateBlackhole();
	void DeactivateBlackhole();
	// 페이즈별 블랙홀 주변 회전
	void BlackholeRotation();
	
	// 소환 횟수 카운트
	int32 SpawnCount;
	
	// 회전 속도값 변수 (블랙홀 가동되면 박스 무게에 따라 설정해줄 값)
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float RotateSpeed = 500.f;

	
};


