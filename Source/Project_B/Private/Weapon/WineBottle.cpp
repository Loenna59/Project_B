#include "Weapon/WineBottle.h"

#include "Character/BaseCharacter.h"
#include "Project_B/Utilities/UTraceChannelHelper.h"

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

	TWeakObjectPtr WeakThis = this;

	UTraceChannelHelper::SphereMultiByChannel
	(
		GetWorld(),
		Location,
		Location,
		FRotator::ZeroRotator,
		ECC_Camera,
		25.f,
		TArray<AActor*>{ this, GetOwner() },
		true,
		[WeakThis](bool bHit, TArray<FHitResult> HitResults)
		{
			if (!WeakThis.IsValid())
			{
				return;
			}

			FVector Location = WeakThis->GetActorLocation();
			
			FVector Direction = (Location - WeakThis->PrevLocation).GetSafeNormal();

			if (bHit)
			{
				for (FHitResult Result : HitResults)
				{
					AActor* HitActor = Result.GetActor();
					if (WeakThis->AlreadyHitActorsDuringAttack.Contains(HitActor))
					{
						continue;
					}
		
					WeakThis->AlreadyHitActorsDuringAttack.Add(HitActor);
					
					if (ABaseCharacter* Character = Cast<ABaseCharacter>(Result.GetActor()))
					{
						Character->OnHit(EAttackType::BOTTLE, Direction, 0);
						WeakThis->DecreaseCapacity();
						break;
					}
				}
			}
		}
	);
	
}

void AWineBottle::SetVisible(bool bVisible, int32 SpawnPointIndex)
{
	Mesh->SetVisibility(bVisible);
	
	Super::SetVisible(bVisible, SpawnPointIndex);
}
