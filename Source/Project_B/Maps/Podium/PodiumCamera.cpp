// Fill out your copyright notice in the Description page of Project Settings.


#include "PodiumCamera.h"

#include "PictureWidget.h"
#include "PodiumMainWidget.h"
#include "Components/SceneCaptureComponent2D.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/KismetRenderingLibrary.h"
#include "Project_B/Utilities/LogMacro.h"


APodiumCamera::APodiumCamera()
{
	CaptureComponent = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("CaptureComponent"));
	CaptureComponent->SetupAttachment(GetRootComponent());
}

void APodiumCamera::BeginPlay()
{
	Super::BeginPlay();
}

void APodiumCamera::Shoot()
{
	LOG_SCREEN("찰칵");
	
	CaptureComponent->TextureTarget = UKismetRenderingLibrary::CreateRenderTarget2D(
		GetWorld(),         
		1920,               
		1080,              
		ETextureRenderTargetFormat::RTF_RGBA16f,
		FLinearColor::Black,// ClearColor
		false,              // bAutoGenerateMipMaps
		false               // bSupportUAVs
	);

	CaptureComponent->CaptureScene();
	CaptureComponent->bCaptureEveryFrame = false;
	
	APlayerController* pc = GetWorld()->GetFirstPlayerController();
	if (pc)
	{
		PictureWidget = CreateWidget<UPictureWidget>(pc, PictureWidgetClass);
		PodiumWidget = CreateWidget<UPodiumMainWidget>(pc, PodiumWidgetClass);
			
		if (PodiumWidget)
		{
			PodiumWidget->AddToViewport();
		}
	}
}
