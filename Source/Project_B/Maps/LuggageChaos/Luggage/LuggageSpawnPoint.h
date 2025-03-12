// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Project_B/Maps/TeamMatchType.h"
#include "LuggageSpawnPoint.generated.h"

UCLASS()
class PROJECT_B_API ALuggageSpawnPoint : public AActor
{
	GENERATED_BODY()

public:
	ALuggageSpawnPoint();

	/**팀 스폰 위치라면, true로 변경*/
	UPROPERTY(EditAnywhere)
	ETeamType Team = ETeamType::Blue;

protected:
	virtual void BeginPlay() override;

public:

};
