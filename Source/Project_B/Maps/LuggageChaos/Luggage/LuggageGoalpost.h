// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Project_B/Maps/TeamMatchType.h"
#include "LuggageGoalpost.generated.h"

class ALuggageManager;

UCLASS()
class PROJECT_B_API ALuggageGoalpost : public AActor
{
	GENERATED_BODY()

public:
	ALuggageGoalpost();

	UPROPERTY(EditAnywhere)
	class UBoxComponent* Box;

	/** RedTeam = false / BlueTeam = true */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Team")
	ETeamType Team = ETeamType::Blue;

private:
	UPROPERTY()
	class ALuggageManager* LuggageManager;

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION()
	void OnBeginOverlapBind(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit);
	UFUNCTION()
	void OnEndOverlapBind(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(Server, Reliable)
	void Server_AddScore(const uint8 point);
	UFUNCTION(NetMulticast, Reliable)
	void Net_AddScore(const uint8 point);
};
