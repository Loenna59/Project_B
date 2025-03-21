// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LuggageScoreWidget.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class PROJECT_B_API ULuggageScoreWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* RedScore;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* BlueScore;

	void UpdateRedScore(const int score);
	void UpdateBlueScore(const int score);
};
