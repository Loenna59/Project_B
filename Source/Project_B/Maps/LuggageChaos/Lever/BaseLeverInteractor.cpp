// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseLeverInteractor.h"

#include "Lever.h"
#include "Project_B/Utilities/LogMacro.h"


// Sets default values
ABaseLeverInteractor::ABaseLeverInteractor()
{
	Root = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Root"));
	SetRootComponent(Root);
	
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(Root);
}

void ABaseLeverInteractor::BeginPlay()
{
	Super::BeginPlay();
	if (Lever)
	{
		Lever->OnLeverValueChanged.AddDynamic(this, &ABaseLeverInteractor::LeverValueInteract);
	}
	else
	{
		LOG_ERROR(this,TEXT("레버 null"));
	}
}

void ABaseLeverInteractor::LeverValueInteract(float value)
{
	LeverValue = value;
}

