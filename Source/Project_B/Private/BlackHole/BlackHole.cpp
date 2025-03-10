// Fill out your copyright notice in the Description page of Project Settings.


#include "BlackHole/BlackHole.h"

#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"


// Sets default values
ABlackHole::ABlackHole()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 외관
	Root = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(Root);
	Sphere = CreateDefaultSubobject<UStaticMeshComponent>("Sphere");
	Sphere->SetupAttachment(RootComponent);
	ConstructorHelpers::FObjectFinder<UStaticMesh>TempBlackHole(TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
	if (TempBlackHole.Succeeded())
	{
		Sphere->SetStaticMesh(TempBlackHole.Object);
	}
	ConstructorHelpers::FObjectFinder<UMaterialInstance>TempBlackHoleMat(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/SkyeMAP/Material/M_BlackHole_Inst.M_BlackHole_Inst'"));
	if (TempBlackHoleMat.Succeeded())
	{
		Sphere->SetMaterial(0, TempBlackHoleMat.Object);
	}
	FirstR_EffectZone = CreateDefaultSubobject<USphereComponent>("FirstR_EffectZone");
	FirstR_EffectZone->SetupAttachment(RootComponent);
	FirstR_EffectZone->SetRelativeScale3D(FVector(3.5f));
	SecondR_OrbitZone = CreateDefaultSubobject<USphereComponent>("SecondR_OrbitZone");
	SecondR_OrbitZone->SetupAttachment(RootComponent);
	SecondR_OrbitZone->SetRelativeScale3D(FVector(3.f));
	ThirdR_DeadZone = CreateDefaultSubobject<USphereComponent>("ThirdR_DeadZone");
	ThirdR_DeadZone->SetupAttachment(RootComponent);
	ThirdR_DeadZone->SetRelativeScale3D(FVector(2.5f));
	FourthR_DistortZone = CreateDefaultSubobject<USphereComponent>("FourthR_DistortZone");
	FourthR_DistortZone->SetupAttachment(RootComponent);
	FourthR_DistortZone->SetRelativeScale3D(FVector(2.0f));
	
}

// Called when the game starts or when spawned
void ABlackHole::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABlackHole::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
}

