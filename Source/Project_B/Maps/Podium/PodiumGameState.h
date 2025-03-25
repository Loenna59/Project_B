// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "PodiumGameState.generated.h"

class UPictureWidget;
/**
 * 
 */
UCLASS()
class PROJECT_B_API APodiumGameState : public AGameStateBase
{
	GENERATED_BODY()
public:
	APodiumGameState();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Camera")
	class APodiumCamera* PodiumCamera = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timer")
	float ReadyTime = 12.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Class")
	TSubclassOf<UPictureWidget> PictureWidgetClass;

private:
	UPROPERTY()
	UPictureWidget* PictureWidget;
	
protected:
	virtual void BeginPlay() override;

public:
	void SetPodiumCamera(APodiumCamera* podiumCamera) { PodiumCamera = podiumCamera; }

private:
	UFUNCTION(NetMulticast, Reliable)
	void Net_Shoot();

	void Shoot();
};
