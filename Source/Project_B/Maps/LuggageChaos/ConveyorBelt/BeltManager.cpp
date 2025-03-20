// Fill out your copyright notice in the Description page of Project Settings.


#include "BeltManager.h"


// Sets default values
ABeltManager::ABeltManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ABeltManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABeltManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

