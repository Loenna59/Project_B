// Fill out your copyright notice in the Description page of Project Settings.


#include "SwingDoor.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"


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
}

void ASwingDoor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASwingDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

