// Fill out your copyright notice in the Description page of Project Settings.


#include "Lever.h"

#include "Components/SphereComponent.h"
#include "Net/UnrealNetwork.h"
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

void ALever::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ALever,LeverValue);
}

void ALever::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (bIsInteracting)
	{
		LeverInteracting(DeltaTime);
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
	Server_IsInteracting(true);
}

void ALever::OnCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//LOG_SCREEN("레버 오버랩 END");
	Server_IsInteracting(false);
}

void ALever::Server_IsInteracting_Implementation(bool isInteract)
{
	Net_IsInteracting(isInteract);
}

void ALever::Net_IsInteracting_Implementation(bool isInteract)
{
	bIsInteracting = isInteract;
	if (isInteract)
	{
		GetWorldTimerManager().ClearTimer(DecreaseValueTimerHandle);
	}
	else
	{
		StartDecreaseValue();
	}
}

void ALever::LeverInteracting(float dt)
{
	float newValue = LeverValue + dt;
	LeverValue = FMath::Clamp(newValue,0,1.0f);
	OnLeverValueChanged.Broadcast(newValue);
}

// void ALever::Server_LeverInteracting_Implementation(float dt)
// {
// 	Net_LeverInteracting(dt);
// }
//
// void ALever::Net_LeverInteracting_Implementation(float dt)
// {
// 	LeverValue+=dt;
// 	float newValue = FMath::Clamp(LeverValue,0,1.0f);
// 	OnLeverValueChanged.Broadcast(newValue);
// }

void ALever::StartDecreaseValue()
{
	GetWorldTimerManager().SetTimer(DecreaseValueTimerHandle, this, &ALever::DecreaseValueStep, 0.025f, true);
}

void ALever::DecreaseValueStep()
{
	LeverValue = FMath::FInterpTo(LeverValue, 0.0f, 0.05f, 1.5f);
	OnLeverValueChanged.Broadcast(LeverValue);
	
	if (FMath::IsNearlyEqual(LeverValue, 0.0f, 0.01f))
	{
		LeverValue = 0.0f;
		GetWorldTimerManager().ClearTimer(DecreaseValueTimerHandle);
	}
}