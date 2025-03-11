// Fill out your copyright notice in the Description page of Project Settings.

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
	UFUNCTION()
	void OnBHBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                      const FHitResult& SweepResult);
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
	
};

