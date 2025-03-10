// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Chain.generated.h"

UCLASS()
class PROJECT_B_API AChain : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AChain();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// 외관
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USceneComponent* Root;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* ChainHandle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* ChainUnit1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* ChainUnit2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* ChainUnit3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* ChainUnit4;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* ChainUnit5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* ChainUnit6;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* ChainUnit7;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* ChainUnit8;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* ChainConnector;
};
