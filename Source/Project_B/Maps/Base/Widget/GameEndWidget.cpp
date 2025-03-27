// Fill out your copyright notice in the Description page of Project Settings.


#include "GameEndWidget.h"

#include "Components/Image.h"

void UGameEndWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UGameEndWidget::ShowVictory()
{
	Victory->SetVisibility(ESlateVisibility::Visible);
}

void UGameEndWidget::ShowDefeat()
{
	Defeat->SetVisibility(ESlateVisibility::Visible);
}

void UGameEndWidget::ShowDraw()
{
	Draw->SetVisibility(ESlateVisibility::Visible);
}
