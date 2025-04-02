// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PodiumMainWidget.generated.h"

class UPictureWidget;
/**
 * 
 */
UCLASS()
class PROJECT_B_API UPodiumMainWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPictureWidget* PictureWidget = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UButton* btn_Finish;
	
	UPROPERTY(Transient,meta = (BindWidgetAnim))
	UWidgetAnimation* FrameAnim;

	void PlayAnimFrame();

	

private:
	UFUNCTION()
	void GoToHome();
	virtual void NativeConstruct() override;
};
