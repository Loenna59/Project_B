#include "Weapon/WineBottle.h"

AWineBottle::AWineBottle()
{
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	SetRootComponent(Mesh);

	ConstructorHelpers::FObjectFinder<UStaticMesh> tempMesh(TEXT("/Game/Assets/_Objects/WineBottle/Winebottle.Winebottle"));

	if (tempMesh.Succeeded())
	{
		Mesh->SetStaticMesh(tempMesh.Object);
	}
}

void AWineBottle::ToggleSimulatePhysics(bool bSimulate)
{
	Super::ToggleSimulatePhysics(bSimulate);
}

void AWineBottle::OnAttackTraceChannel()
{
	Super::OnAttackTraceChannel();
}
