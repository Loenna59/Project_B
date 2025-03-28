// Fill out your copyright notice in the Description page of Project Settings.


#include "WinnerPrize.h"


AWinnerPrize::AWinnerPrize()
{
	PrimaryActorTick.bCanEverTick = true;

	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMesh");
	RootComponent = SkeletalMesh;

	bReplicates = true;
	SetReplicates(true);
}

void AWinnerPrize::BeginPlay()
{
	Super::BeginPlay();
	
}

void AWinnerPrize::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

