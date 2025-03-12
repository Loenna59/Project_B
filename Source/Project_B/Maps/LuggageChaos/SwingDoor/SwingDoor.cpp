// Fill out your copyright notice in the Description page of Project Settings.


#include "SwingDoor.h"


ASwingDoor::ASwingDoor()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ASwingDoor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASwingDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

