// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Project_B/Maps/BanimalsType.h"
#include "LuggageSpawnPoint.generated.h"

UCLASS()
class PROJECT_B_API ALuggageSpawnPoint : public AActor
{
	GENERATED_BODY()

public:
	ALuggageSpawnPoint();
	
	UPROPERTY(EditAnywhere)
	ETeamType Team = ETeamType::Blue;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* Root;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UArrowComponent* ThrowArrow;

protected:
	virtual void BeginPlay() override;



};
