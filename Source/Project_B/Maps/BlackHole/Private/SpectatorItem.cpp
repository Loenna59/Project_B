// Fill out your copyright notice in the Description page of Project Settings.


#include "Project_B/Maps/BlackHole/Public/SpectatorItem.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "WorldPartition/ContentBundle/ContentBundleLog.h"


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
	ProjectileMovement->Bounciness = 0; // 탄성계수
	ProjectileMovement->Friction = 1; // 마찰력(1이 최대)

	SpectatorMesh->OnComponentHit.AddDynamic(this, &ASpectatorItem::OnHit);
}

void ASpectatorItem::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// 완전히 멈추려면:
	//ProjectileMovement->StopMovementImmediately();
    
	// 또는 속도를 크게 줄이려면:
	ProjectileMovement->Velocity *= 0.1f; // 속도를 90% 감소
	UE_LOG(LogTemp, Warning, TEXT("ProjectileMovement->HIT!!!!!!"));
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

