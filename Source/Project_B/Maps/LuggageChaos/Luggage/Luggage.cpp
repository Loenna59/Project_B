

#include "Luggage.h"


ALuggage::ALuggage()
{
	PrimaryActorTick.bCanEverTick = true;

	LuggageMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LuggageMesh"));
	SetRootComponent(LuggageMesh);
	
	LuggageMesh->SetCollisionObjectType(ECC_WorldDynamic);
	LuggageMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	LuggageMesh->SetSimulatePhysics(true);
	
	LuggageMesh->SetUseCCD(true);

	bReplicates = true;
	SetReplicateMovement(true);
}

void ALuggage::BeginPlay()
{
	Super::BeginPlay();
	if (HasAuthority() == false)
	{
		SetActorEnableCollision(false);
	}
}

void ALuggage::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

