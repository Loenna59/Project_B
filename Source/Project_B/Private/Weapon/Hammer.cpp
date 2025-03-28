// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon/Hammer.h"

#include "Components/CapsuleComponent.h"

// Sets default values
AHammer::AHammer()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	SetRootComponent(Mesh);

	Trigger = CreateDefaultSubobject<UCapsuleComponent>(TEXT("TriggerComp"));
	Trigger->SetupAttachment(Mesh);

	ConstructorHelpers::FObjectFinder<UStaticMesh> tempMesh(TEXT("/Game/Assets/_Objects/Hammer/Hammer1.Hammer1"));

	if (tempMesh.Succeeded())
	{
		Mesh->SetStaticMesh(tempMesh.Object);
	}
}

// Called when the game starts or when spawned
void AHammer::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHammer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

