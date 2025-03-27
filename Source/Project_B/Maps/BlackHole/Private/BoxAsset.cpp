// Fill out your copyright notice in the Description page of Project Settings.


#include "Project_B/Maps/BlackHole/Public/BoxAsset.h"

#include "Project_B/Utilities/LogMacro.h"

// Sets default values
ABoxAsset::ABoxAsset()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Box = CreateDefaultSubobject<UStaticMeshComponent>("Box");
	SetRootComponent(Box);
	Box->SetSimulatePhysics(true);

	// TODO: 확인
	Box->bReplicatePhysicsToAutonomousProxy = true;
}

void ABoxAsset::OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	if (HitComponent->IsSimulatingPhysics())
	{
		HitComponent->SetAllPhysicsLinearVelocity(FVector::ZeroVector);
	}
}

// Called when the game starts or when spawned
void ABoxAsset::BeginPlay()
{
	Super::BeginPlay();

	// 동기화
	SetReplicates(true);
	SetReplicateMovement(true);

	Box->OnComponentHit.AddDynamic(this, &ABoxAsset::OnComponentHit);
}

// Called every frame
void ABoxAsset::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector Velocity = Box->GetPhysicsLinearVelocity();

	float MaxSpeed = 100.f;
	if (Velocity.Size() > MaxSpeed)
	{
		Velocity = Velocity.GetClampedToMaxSize(MaxSpeed);
		Box->SetPhysicsLinearVelocity(Velocity);
	}
}

