// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PictureWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_B_API UPictureWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	class UImage* Photo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UTextureRenderTarget2D* RenderTarget;
	
};
