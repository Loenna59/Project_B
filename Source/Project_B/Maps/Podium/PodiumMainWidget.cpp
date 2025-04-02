// Fill out your copyright notice in the Description page of Project Settings.


#include "PodiumMainWidget.h"

#include "PodiumGameMode.h"
#include "PodiumGameState.h"
#include "Components/Button.h"

void UPodiumMainWidget::PlayAnimFrame()
{
	PlayAnimation(FrameAnim);
}

void UPodiumMainWidget::GoToHome()
{
	APodiumGameMode* gm = Cast<APodiumGameMode>(GetWorld()->GetAuthGameMode());
	gm->GoToHome();
}

void UPodiumMainWidget::NativeConstruct()
{
	Super::NativeConstruct();
	PlayAnimation(FrameAnim);

	if (btn_Finish)
	{
		btn_Finish->OnClicked.AddDynamic(this, &UPodiumMainWidget::GoToHome);
	}
}
