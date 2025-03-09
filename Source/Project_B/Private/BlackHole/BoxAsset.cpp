// Fill out your copyright notice in the Description page of Project Settings.


#include "Project_B/Public/BlackHole/BoxAsset.h"


// Sets default values
ABoxAsset::ABoxAsset()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Box = CreateDefaultSubobject<UStaticMeshComponent>("Box");
	SetRootComponent(Box);
	Box->SetSimulatePhysics(true);
}

// Called when the game starts or when spawned
void ABoxAsset::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABoxAsset::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

