// Fill out your copyright notice in the Description page of Project Settings.


#include "LuggageSpawnPoint.h"

#include "Components/ArrowComponent.h"


ALuggageSpawnPoint::ALuggageSpawnPoint()
{
	Root = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(Root);
	
	ThrowArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	ThrowArrow->SetupAttachment(Root);
}

// Called when the game starts or when spawned
void ALuggageSpawnPoint::BeginPlay()
{
	Super::BeginPlay();
}
