// Fill out your copyright notice in the Description page of Project Settings.


#include "SwingDoor.h"

#include "Components/BoxComponent.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "Project_B/Maps/LuggageChaos/Luggage/Luggage.h"
#include "Project_B/Utilities/LogMacro.h"


ASwingDoor::ASwingDoor()
{
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Root"));
	SetRootComponent(Root);

	LeftDoor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftDoor"));
	LeftDoor->SetupAttachment(Root);
	RightDoor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightDoor"));
	RightDoor->SetupAttachment(Root);
	LeftGlass = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftGlass"));
	LeftGlass->SetupAttachment(LeftDoor);
	RightGlass = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightGlass"));
	RightGlass->SetupAttachment(RightDoor);

	LeftPivot = CreateDefaultSubobject<USceneComponent>(TEXT("LeftPivot"));
	LeftPivot->SetupAttachment(Root);
	RightPivot = CreateDefaultSubobject<USceneComponent>(TEXT("RightPivot"));
	RightPivot->SetupAttachment(Root);

	LeftConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("LeftConstraint"));
	LeftConstraint->SetupAttachment(Root);
	RightConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("RightConstraint"));
	RightConstraint->SetupAttachment(Root);

	LeftDoor->SetSimulatePhysics(true);
	RightDoor->SetSimulatePhysics(true);

	LeftConstraint->SetAngularSwing1Limit(ACM_Limited,75.0f);
	RightConstraint->SetAngularSwing1Limit(ACM_Limited,75.0f);

	LeftConstraint->SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Limited, 75.f);
	LeftConstraint->SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Locked, 0.f);
	LeftConstraint->SetAngularTwistLimit(EAngularConstraintMotion::ACM_Locked, 0.f);
	RightConstraint->SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Limited, 75.f);
	RightConstraint->SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Locked, 0.f);
	RightConstraint->SetAngularTwistLimit(EAngularConstraintMotion::ACM_Locked, 0.f);

	LeftConstraint->SetAngularDriveMode(EAngularDriveMode::TwistAndSwing);
	LeftConstraint->SetOrientationDriveTwistAndSwing(false,true);
	LeftConstraint->SetAngularDriveParams(200.f, 1.f, 0.f);
	
	RightConstraint->SetAngularDriveMode(EAngularDriveMode::TwistAndSwing);
	RightConstraint->SetOrientationDriveTwistAndSwing(false,true);
	RightConstraint->SetAngularDriveParams(200.f, 1.f, 0.f);

	BoxCollison = CreateDefaultSubobject<UBoxComponent>(TEXT("SphereCollison"));
	BoxCollison->SetupAttachment(Root);

	bReplicates = true;
}

void ASwingDoor::BeginPlay()
{
	Super::BeginPlay();
	BoxCollison->OnComponentBeginOverlap.AddDynamic(this,&ASwingDoor::OnCollisionBeginOverlap);
	BoxCollison->OnComponentEndOverlap.AddDynamic(this,&ASwingDoor::OnCollisionEndOverlap);
	
}

void ASwingDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASwingDoor::OnCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA(ALuggage::StaticClass()))
	{
		++NearLugCount;
		DoorLocked();
		//LOG_SCREEN("캐리어 근처");
	}
}

void ASwingDoor::OnCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->IsA(ALuggage::StaticClass()))
	{
		--NearLugCount;
	}
	
	if (NearLugCount <= 0)
	{
		DoorUnlocked();
		//LOG_SCREEN("캐리어 근처 아님");
	}
}

void ASwingDoor::DoorLocked()
{
	LeftConstraint->SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Locked, 1.f);
	RightConstraint->SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Locked, 1.f);
}

void ASwingDoor::DoorUnlocked()
{
	LeftConstraint->SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Limited, 75.f);
	RightConstraint->SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Limited, 75.f);
}
