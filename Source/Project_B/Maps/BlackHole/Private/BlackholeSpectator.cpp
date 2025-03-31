// Fill out your copyright notice in the Description page of Project Settings.


#include "Project_B/Maps/BlackHole/Public/BlackholeSpectator.h"

#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"


class UEnhancedInputLocalPlayerSubsystem;
// Sets default values
ABlackholeSpectator::ABlackholeSpectator()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpectatorCam = CreateDefaultSubobject<UCameraComponent>(TEXT("SpectatorCam"));
	SpectatorCam->FieldOfView = 55.0f;
	SpectatorCam->SetWorldLocationAndRotation(FVector(0,-2620,1140), FRotator(-20, 90, 0));
}

// Called when the game starts or when spawned
void ABlackholeSpectator::BeginPlay()
{
	Super::BeginPlay();

	pc = Cast<APlayerController>(GetController());
	bAddDefaultMovementBindings = false;
}

// Called every frame
void ABlackholeSpectator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ABlackholeSpectator::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ABlackholeSpectator::SpawnProjectile);
}

void ABlackholeSpectator::SpawnProjectile()
{
	if (!GetController()) return;
	if (!pc) return;

	FVector Location;
	FRotator Rotation;
	pc->GetPlayerViewPoint(Location, Rotation);
	
	// Projectile 발사 위치
	FVector SpawnLocation = Location + Rotation.Vector() * 100.0f;
	FActorSpawnParameters SpawnParams;

	UE_LOG(LogTemp, Warning, TEXT("Spawning Projectile"));
	// Projectile 생성
	// GetWorld()->SpawnActor<AProjectileClass>(ProjectileClass, SpawnLocation, Rotation, SpawnParams);
}

