// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraActor.h"
#include "PodiumCamera.generated.h"

class UPictureWidget;

UCLASS()
class PROJECT_B_API APodiumCamera : public ACameraActor
{
	GENERATED_BODY()

public:
	APodiumCamera();
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BeginDelay = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	USceneCaptureComponent2D* CaptureComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	TSubclassOf<UPictureWidget> PictureWidgetClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Class")
	TSubclassOf<class UPodiumMainWidget> PodiumWidgetClass;
private:
	UPROPERTY()
	UPictureWidget* PictureWidget;
	
	UPROPERTY()
	UPodiumMainWidget* PodiumWidget;

protected:
	virtual void BeginPlay() override;

	void SetPodiumCamera();
	
public:
	void Shoot();
};
