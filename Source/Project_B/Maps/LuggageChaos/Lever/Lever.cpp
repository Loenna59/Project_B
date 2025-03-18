// Fill out your copyright notice in the Description page of Project Settings.


#include "Lever.h"

#include "Components/SphereComponent.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "Project_B/Utilities/LogMacro.h"

ALever::ALever()
{
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(Root);

	LeverMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeverMesh"));
	LeverMesh->SetupAttachment(Root);

	LeverPivot = CreateDefaultSubobject<USceneComponent>(TEXT("LeverPivot"));
	LeverPivot->SetupAttachment(Root);

	LeverConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("LeverConstraint"));
	LeverConstraint->SetupAttachment(Root);
	
	SphereCollision = CreateDefaultSubobject<USphereComponent>("SphereCollision");
	SphereCollision->SetupAttachment(LeverMesh);
	SphereCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereCollision->SetCollisionResponseToChannel(ECC_Pawn,ECR_Overlap);
}

void ALever::BeginPlay()
{
	Super::BeginPlay();
	SphereCollision->OnComponentBeginOverlap.AddDynamic(this,&ALever::OnCollisionBeginOverlap);
	SphereCollision->OnComponentEndOverlap.AddDynamic(this,&ALever::OnCollisionEndOverlap);
}

void ALever::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (bIsInteracting)
	{
		LeverValue+=DeltaTime;
		float newValue = FMath::Clamp(LeverValue,0,1.0f);
		OnLeverValueChanged.Broadcast(newValue);
	}
}

void ALever::SetLeverValue(float value)
{
	LeverValue = value;
}

void ALever::OnCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                     UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//LOG_SCREEN("레버 오버랩 BEGIN");
	bIsInteracting = true;
	GetWorldTimerManager().ClearTimer(DecreaseValueTimerHandle);
}

void ALever::OnCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//LOG_SCREEN("레버 오버랩 END");
	bIsInteracting = false;
	StartDecreaseValue();
}

void ALever::StartDecreaseValue()
{
	GetWorldTimerManager().SetTimer(DecreaseValueTimerHandle, this, &ALever::DecreaseValueStep, 0.025f, true);
}

void ALever::DecreaseValueStep()
{
	LeverValue = FMath::FInterpTo(LeverValue, 0.0f, 0.05f, 1);
	OnLeverValueChanged.Broadcast(LeverValue);
	
	if (FMath::IsNearlyEqual(LeverValue, 0.0f, 0.01f))
	{
		LeverValue = 0.0f;
		GetWorldTimerManager().ClearTimer(DecreaseValueTimerHandle);
	}
}