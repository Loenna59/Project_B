// Fill out your copyright notice in the Description page of Project Settings.


#include "PodiumGameState.h"

#include "PodiumCamera.h"
#include "Blueprint/UserWidget.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Project_B/Maps/Podium/PictureWidget.h"
#include "Project_B/Utilities/LogMacro.h"

APodiumGameState::APodiumGameState()
{
}

void APodiumGameState::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		FTimerHandle OnStartTimerHandle;
		
		GetWorld()->GetTimerManager().SetTimer(OnStartTimerHandle, this, &APodiumGameState::Net_Shoot,ReadyTime,false);
	}
}

void APodiumGameState::Shoot()
{
	if (PodiumCamera)
	{
		PodiumCamera->Shoot();
	}
	else
	{
		LOG_ERROR(this, TEXT("포디엄 카메라 is Null"));
	}
}

void APodiumGameState::Net_Shoot_Implementation()
{
	if (PodiumCamera)
	{
		PodiumCamera->Shoot();
	}
	else
	{
		LOG_ERROR(this, TEXT("포디엄 카메라 is Null"));
	}
}
