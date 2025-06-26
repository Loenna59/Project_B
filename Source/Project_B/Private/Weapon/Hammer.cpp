#include "Weapon/Hammer.h"

#include "Character/BaseCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Project_B/Utilities/UTraceChannelHelper.h"

AHammer::AHammer()
{
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	SetRootComponent(Mesh);

	Trigger = CreateDefaultSubobject<UCapsuleComponent>(TEXT("TriggerComp"));
	Trigger->SetupAttachment(Mesh);

	ConstructorHelpers::FObjectFinder<UStaticMesh> tempMesh(TEXT("/Game/Assets/_Objects/Hammer/Hammer1.Hammer1"));

	if (tempMesh.Succeeded())
	{
		Mesh->SetStaticMesh(tempMesh.Object);
	}

	HitPoint = CreateDefaultSubobject<USceneComponent>(TEXT("HitPoint"));
	HitPoint->SetupAttachment(Mesh);
	HitPoint->SetRelativeLocation(FVector(0, 150, -150.f));
}

void AHammer::ToggleSimulatePhysics(bool bSimulate)
{
	Super::ToggleSimulatePhysics(bSimulate);

	if (Mesh)
	{
		Mesh->SetSimulatePhysics(bSimulate);
	}

	ECollisionEnabled::Type CollisionEnabled = bSimulate? ECollisionEnabled::QueryAndPhysics : ECollisionEnabled::NoCollision;
	
	Mesh->SetCollisionEnabled(CollisionEnabled);
	Trigger->SetCollisionEnabled(CollisionEnabled);
}

void AHammer::OnAttackTraceChannel()
{
	Super::OnAttackTraceChannel();

	FVector Location = HitPoint->GetComponentLocation();

	TWeakObjectPtr WeakThis = this;

	UTraceChannelHelper::SphereMultiByChannel
	(
		GetWorld(),
		Location,
		Location,
		FRotator::ZeroRotator,
		ECC_Camera,
		50.f,
		TArray<AActor*>{this, GetOwner()},
		true,
		FOnMultiTraceCompleted::CreateLambda
		(
			[WeakThis](bool bHit, TArray<FHitResult> HitResults)
			{
				if (!WeakThis.IsValid())
				{
					return;
				}
				
				FVector Location = WeakThis->HitPoint->GetComponentLocation();
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
							Character->OnHit(EAttackType::HAMMER, Direction, 0);
							WeakThis->DecreaseCapacity();
							break;
						}
					}
				}
			
				WeakThis->PrevLocation = Location;
			}
		)
	);
	
}

void AHammer::SetVisible(bool bVisible, int32 SpawnPointIndex)
{
	Mesh->SetVisibility(bVisible);
	HitPoint->SetActive(bVisible);
	
	Super::SetVisible(bVisible, SpawnPointIndex);
}

