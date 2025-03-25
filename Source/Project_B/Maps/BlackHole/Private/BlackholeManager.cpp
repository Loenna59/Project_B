// Fill out your copyright notice in the Description page of Project Settings.


#include "Project_B/Maps/BlackHole/Public/BlackholeManager.h"


// Sets default values
ABlackholeManager::ABlackholeManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ABlackholeManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABlackholeManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

