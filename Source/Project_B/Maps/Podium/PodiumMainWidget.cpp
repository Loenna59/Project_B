// Fill out your copyright notice in the Description page of Project Settings.


#include "PodiumMainWidget.h"

void UPodiumMainWidget::PlayAnimFrame()
{
	PlayAnimation(FrameAnim);
}

void UPodiumMainWidget::NativeConstruct()
{
	Super::NativeConstruct();
	PlayAnimation(FrameAnim);
}
