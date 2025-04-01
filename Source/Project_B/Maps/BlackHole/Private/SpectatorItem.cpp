// Fill out your copyright notice in the Description page of Project Settings.


#include "Project_B/Maps/BlackHole/Public/SpectatorItem.h"


// Sets default values
ASpectatorItem::ASpectatorItem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ASpectatorItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASpectatorItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

