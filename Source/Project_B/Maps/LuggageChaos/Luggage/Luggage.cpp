

#include "Luggage.h"


ALuggage::ALuggage()
{
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	LuggageMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LuggageMesh"));
	LuggageMesh->SetupAttachment(Root);
	LuggageMesh->SetCollisionObjectType(ECC_PhysicsBody);
	LuggageMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	LuggageMesh->SetSimulatePhysics(true);
}

void ALuggage::BeginPlay()
{
	Super::BeginPlay();
	
}

void ALuggage::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

