// Fill out your copyright notice in the Description page of Project Settings.


#include "Project_B/Maps/BlackHole/Public/SpectatorActor.h"


// Sets default values
ASpectatorActor::ASpectatorActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ASpectatorActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASpectatorActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

