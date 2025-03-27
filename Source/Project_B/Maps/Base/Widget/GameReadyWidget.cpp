// Fill out your copyright notice in the Description page of Project Settings.


#include "GameReadyWidget.h"

void UGameReadyWidget::PlayAnimLoadComplete()
{
	PlayAnimation(LoadComplete);
}

void UGameReadyWidget::OnAnimationFinished_Implementation(const UWidgetAnimation* Animation)
{
	Super::OnAnimationFinished_Implementation(Animation);
}
