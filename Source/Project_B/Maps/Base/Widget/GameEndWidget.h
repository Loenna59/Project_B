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
	
	UPROPERTY(meta = (BindWidget))
	UImage* Victory;
	UPROPERTY(meta = (BindWidget))
	UImage* Defeat;
	UPROPERTY(meta = (BindWidget))
	UImage* Draw;

public:
	UFUNCTION()
	void ShowVictory();
	UFUNCTION()
	void ShowDefeat();
	UFUNCTION()
	void ShowDraw();
};
