// Fill out your copyright notice in the Description page of Project Settings.


#include "LuggageScoreWidget.h"

#include <string>

#include "Components/TextBlock.h"

void ULuggageScoreWidget::UpdateRedScore(const int score)
{
	RedScore->SetText(FText::AsNumber(score));
}

void ULuggageScoreWidget::UpdateBlueScore(const int score)
{
	BlueScore->SetText(FText::AsNumber(score));
}
