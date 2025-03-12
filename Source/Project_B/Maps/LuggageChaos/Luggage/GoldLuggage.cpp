// Fill out your copyright notice in the Description page of Project Settings.


#include "GoldLuggage.h"


// Sets default values
AGoldLuggage::AGoldLuggage()
{
	PrimaryActorTick.bCanEverTick = true;

	Point = 4;
	Weight *= 2;
}

void AGoldLuggage::BeginPlay()
{
	Super::BeginPlay();
	
}

void AGoldLuggage::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

