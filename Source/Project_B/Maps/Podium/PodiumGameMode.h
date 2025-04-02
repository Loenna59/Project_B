// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PodiumGameMode.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_B_API APodiumGameMode : public AGameModeBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BeginDelay = 1.2f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Class | Sound")
	USoundWave* BGM;
	
	UPROPERTY()
	UAudioComponent* BgmComponent;
	
private:
	TArray<FString> WinnerKeys;

public:
	void GoToHome();
	
private:
	virtual void BeginPlay() override;
	virtual void OnPostLogin(AController* NewPlayer) override;
	
	UFUNCTION(Server, Reliable)
	void Server_GoToHome();
};
