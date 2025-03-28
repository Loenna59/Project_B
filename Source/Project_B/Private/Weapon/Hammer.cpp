// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon/Hammer.h"

#include "KismetTraceUtils.h"
#include "Character/BaseCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Project_B/Utilities/TraceChannelHelper.h"

// Sets default values
AHammer::AHammer()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
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
		FCollisionShape::MakeBox(FVector(100.f)),
		CollisionParams
	);

	if (bDrawDebug)
	{
		DrawDebugBoxTraceMulti(
			GetWorld(),
			Location,
			Location,
			FVector(100.f),
			FRotator::ZeroRotator,
			EDrawDebugTrace::ForDuration,
			bHit,
			HitResult,
			FColor::Yellow,
			FColor::Green,
			1.f
		);
	}

	if (bHit)
	{
		for (FHitResult Result : HitResult)
		{
			if (ABaseCharacter* Character = Cast<ABaseCharacter>(Result.GetActor()))
			{
				Character->OnHit(EAttackType::HAMMER, Result.Normal.GetSafeNormal(), 0);
				break;
			}
		}
	}
}

