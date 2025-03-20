// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BeltManager.generated.h"

class AConveyorBelt;
class AConveyorBeltLong;

UCLASS()
class PROJECT_B_API ABeltManager : public AActor
{
	GENERATED_BODY()

public:
	ABeltManager();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Belt")
	TArray<AConveyorBelt*> ShortBelts;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Belt")
	TArray<AConveyorBeltLong*> LongBelts;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move")
	float Speed = 100.0f;

	UPROPERTY(EditAnywhere)
	bool bIsMoving = false;
	
protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(NetMulticast, reliable)
	void Net_StartMoving();
};
