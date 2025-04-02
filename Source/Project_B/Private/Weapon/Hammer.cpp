#include "Weapon/Hammer.h"

#include "KismetTraceUtils.h"
#include "Character/BaseCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Project_B/Utilities/LogMacro.h"
#include "Project_B/Utilities/TraceChannelHelper.h"

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
		FCollisionShape::MakeSphere(50.f),
		CollisionParams
	);

	if (bDrawDebug)
	{
		// DrawDebugSphereTraceMulti(
		// 	GetWorld(),
		// 	Location,
		// 	Location,
		// 	50.f,
		// 	EDrawDebugTrace::ForDuration,
		// 	bHit,
		// 	HitResult,
		// 	FColor::Yellow,
		// 	FColor::Green,
		// 	1.f
		// );
	}

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
				Character->OnHit(EAttackType::HAMMER, Result.Normal.GetSafeNormal(), 0);
				DecreaseCapacity();
				break;
			}
		}
	}
}

void AHammer::SetVisible(bool bVisible, int32 SpawnPointIndex)
{
	Mesh->SetVisibility(bVisible);
	HitPoint->SetActive(bVisible);
	
	Super::SetVisible(bVisible, SpawnPointIndex);
}

