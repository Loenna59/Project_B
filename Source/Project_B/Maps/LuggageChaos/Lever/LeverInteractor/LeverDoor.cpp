// Fill out your copyright notice in the Description page of Project Settings.


#include "LeverDoor.h"

#include "Project_B/Utilities/LogMacro.h"


ALeverDoor::ALeverDoor()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ALeverDoor::BeginPlay()
{
	Super::BeginPlay();
}

void ALeverDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ALeverDoor::LeverValueInteract(float value)
{
	StaticMesh->SetRelativeRotation(FRotator(value*(-90.0f), 0,0));
}

