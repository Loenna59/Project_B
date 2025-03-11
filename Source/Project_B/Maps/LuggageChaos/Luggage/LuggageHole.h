// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LuggageHole.generated.h"

UCLASS()
class PROJECT_B_API ALuggageHole : public AActor
{
	GENERATED_BODY()

public:
	ALuggageHole();

	UPROPERTY(EditAnywhere)
	class UBoxComponent* Box;

	/** RedTeam = false / BlueTeam = true */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Team")
	bool Team = false;

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION()
	void OnBeginOverlapBind(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit);
	UFUNCTION()
	void OnEndOverlapBind(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
};
