// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameEndWidget.generated.h"

class UImage;
/**
 * 
 */
UCLASS()
class PROJECT_B_API UGameEndWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	
	UPROPERTY(Transient,meta = (BindWidgetAnim))
	UWidgetAnimation* AnimVictory;
	UPROPERTY(Transient,meta = (BindWidgetAnim))
	UWidgetAnimation* AnimDefeat;
	UPROPERTY(Transient,meta = (BindWidgetAnim))
	UWidgetAnimation* AnimDraw;

public:
	UFUNCTION()
	void ShowVictory();
	UFUNCTION()
	void ShowDefeat();
	UFUNCTION()
	void ShowDraw();
};
