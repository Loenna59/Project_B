#include "Weapon/WineBottle.h"

#include "KismetTraceUtils.h"
#include "Character/BaseCharacter.h"
#include "Project_B/Utilities/LogMacro.h"

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

	if (Mesh)
	{
		Mesh->SetSimulatePhysics(bSimulate);
	}

	ECollisionEnabled::Type CollisionEnabled = bSimulate? ECollisionEnabled::QueryAndPhysics : ECollisionEnabled::NoCollision;
	
	Mesh->SetCollisionEnabled(CollisionEnabled);
}

void AWineBottle::OnAttackTraceChannel()
{
	Super::OnAttackTraceChannel();

	FVector Location = GetActorLocation();

	TArray<FHitResult> HitResult;
	FCollisionQueryParams CollisionParams;

	CollisionParams.AddIgnoredActor(this);
	CollisionParams.AddIgnoredActor(GetOwner());
	
	bool bHit = GetWorld()->SweepMultiByChannel(
		HitResult,
		Location,
		Location,
		FQuat::Identity,
		ECC_Camera,
		FCollisionShape::MakeSphere(25.f),
		CollisionParams
	);

	if (bDrawDebug)
	{
		// DrawDebugSphereTraceMulti(
		// 	GetWorld(),
		// 	Location,
		// 	Location,
		// 	25.f,
		// 	EDrawDebugTrace::ForDuration,
		// 	bHit,
		// 	HitResult,
		// 	FColor::Yellow,
		// 	FColor::Green,
		// 	1.f
		// );
	}

	FVector Direction = (Location - PrevLocation).GetSafeNormal();

	if (bHit)
	{
		for (FHitResult Result : HitResult)
		{
			AActor* HitActor = Result.GetActor();
			if (AlreadyHitActorsDuringAttack.Contains(HitActor))
			{
				continue;
			}

			AlreadyHitActorsDuringAttack.Add(HitActor);
			
			if (ABaseCharacter* Character = Cast<ABaseCharacter>(Result.GetActor()))
			{
				Character->OnHit(EAttackType::BOTTLE, Direction, 0);
				DecreaseCapacity();
				break;
			}
		}
	}
}

void AWineBottle::SetVisible(bool bVisible, int32 SpawnPointIndex)
{
	Mesh->SetVisibility(bVisible);
	
	Super::SetVisible(bVisible, SpawnPointIndex);
}
