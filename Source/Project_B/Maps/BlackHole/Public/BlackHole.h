
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
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	// 스폰되었니? 변수
	// 변수가 자동으로 동기화?
	UPROPERTY(Replicated)
	bool bIsActive = false;

	// 생성되기 10초전부터 블랙홀의 scale 바꿔주자 (커지면서 생성되게)
	UPROPERTY()
	float CurrentScale = 0.0f;
	
	// 외관
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USceneComponent* Root;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* Sphere;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USphereComponent* SphereComp;

	// 체인
	void ActivateBlackhole();
	UPROPERTY()
	FVector GravityVel;
	
};


