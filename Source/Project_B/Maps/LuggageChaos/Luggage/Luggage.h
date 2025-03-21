// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Luggage.generated.h"

UCLASS()
class PROJECT_B_API ALuggage : public AActor
{
	GENERATED_BODY()

public:
	ALuggage();
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UStaticMeshComponent* LuggageMesh;

	/**일반 러기지 1점, 황금 러기지 4점*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Luggage")
	uint8 Point = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Luggage")
	float Weight;
	
protected:
	

private:
	
	
protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
};
