// Fill out your copyright notice in the Description page of Project Settings.


#include "Project_B/Maps/BlackHole/Public/SpectatorItem.h"

#include "GameFramework/ProjectileMovementComponent.h"


// Sets default values
ASpectatorItem::ASpectatorItem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	
	SpectatorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SpectatorMesh"));
	SetRootComponent(SpectatorMesh);
	SpectatorMesh->SetSimulatePhysics(true);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->SetUpdatedComponent(SpectatorMesh);
	ProjectileMovement->InitialSpeed = 3000;
	ProjectileMovement->MaxSpeed = 3000;
	ProjectileMovement->bShouldBounce = false;
	// ProjectileMovement->Bounciness = 0.3f;
}

// Called when the game starts or when spawned
void ASpectatorItem::BeginPlay()
{
	Super::BeginPlay();
	
	SetReplicates(true);
	SetReplicateMovement(true);
}

// Called every frame
void ASpectatorItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

