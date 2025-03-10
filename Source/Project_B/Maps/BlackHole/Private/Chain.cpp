// Fill out your copyright notice in the Description page of Project Settings.


#include "Project_B/Maps/BlackHole/Public/Chain.h"


// Sets default values
AChain::AChain()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 외관
	Root = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(Root);
	// 핸들 부분
	ChainHandle = CreateDefaultSubobject<UStaticMeshComponent>("ChainHandle");
	ChainHandle->SetupAttachment(RootComponent);
	ChainHandle->SetRelativeRotation(FRotator(0, 90, 0));
	ChainHandle->SetSimulatePhysics(true);
	ConstructorHelpers::FObjectFinder<UStaticMesh>TempHandle(TEXT("/Script/Engine.StaticMesh'/Game/Assets/Blackhole/ChainHandle.ChainHandle'"));
	if (TempHandle.Succeeded())
	{
		ChainHandle->SetStaticMesh(TempHandle.Object);
	}
	
	// 체인 머리
	ChainConnector = CreateDefaultSubobject<UStaticMeshComponent>("ChainConnector");
	ChainConnector->SetupAttachment(RootComponent);
	ChainConnector->SetRelativeLocation(FVector(365, 0, -45));
	ChainConnector->SetRelativeRotation(FRotator(0, 90, 0));
	ConstructorHelpers::FObjectFinder<UStaticMesh>TempConnector(TEXT("/Script/Engine.StaticMesh'/Game/Assets/Blackhole/ChainConnector.ChainConnector'"));
	if (TempConnector.Succeeded())
	{
		ChainConnector->SetStaticMesh(TempConnector.Object);
	}
	
	// 체인 유닛 부분
	ChainUnit1 = CreateDefaultSubobject<UStaticMeshComponent>("Chain1");
	ChainUnit1->SetupAttachment(RootComponent);
	ChainUnit1->SetRelativeLocationAndRotation(FVector(10,0,0),FRotator(90,180,90));
	ChainUnit1->SetSimulatePhysics(true);
	
	ChainUnit2 = CreateDefaultSubobject<UStaticMeshComponent>("Chain2");
	ChainUnit2->SetupAttachment(RootComponent);
	ChainUnit2->SetRelativeLocationAndRotation(FVector(50,0,0),FRotator(0,90,0));
	ChainUnit2->SetSimulatePhysics(true);
	
	ChainUnit3 = CreateDefaultSubobject<UStaticMeshComponent>("Chain3");
	ChainUnit3->SetupAttachment(RootComponent);
	ChainUnit3->SetRelativeLocationAndRotation(FVector(90,0,0),FRotator(90,180,90));
	ChainUnit3->SetSimulatePhysics(true);

	ChainUnit4 = CreateDefaultSubobject<UStaticMeshComponent>("Chain4");
	ChainUnit4->SetupAttachment(RootComponent);
	ChainUnit4->SetRelativeLocationAndRotation(FVector(130,0,0),FRotator(0,90,0));
	ChainUnit4->SetSimulatePhysics(true);

	ChainUnit5 = CreateDefaultSubobject<UStaticMeshComponent>("Chain5");
	ChainUnit5->SetupAttachment(RootComponent);
	ChainUnit5->SetRelativeLocationAndRotation(FVector(170,0,0),FRotator(90,180,90));
	ChainUnit5->SetSimulatePhysics(true);

	ChainUnit6 = CreateDefaultSubobject<UStaticMeshComponent>("Chain6");
	ChainUnit6->SetupAttachment(RootComponent);
	ChainUnit6->SetRelativeLocationAndRotation(FVector(210,0,0),FRotator(0,90,0));
	ChainUnit6->SetSimulatePhysics(true);

	ChainUnit7 = CreateDefaultSubobject<UStaticMeshComponent>("Chain7");
	ChainUnit7->SetupAttachment(RootComponent);
	ChainUnit7->SetRelativeLocationAndRotation(FVector(250,0,0),FRotator(90,180,90));
	ChainUnit7->SetSimulatePhysics(true);

	ChainUnit8 = CreateDefaultSubobject<UStaticMeshComponent>("Chain8");
	ChainUnit8->SetupAttachment(RootComponent);
	ChainUnit8->SetRelativeLocationAndRotation(FVector(290,0,0),FRotator(0,90,0));
	ChainUnit8->SetSimulatePhysics(true);

	ConstructorHelpers::FObjectFinder<UStaticMesh>TempUnit(TEXT("/Script/Engine.StaticMesh'/Game/Assets/Blackhole/ChainUnit.ChainUnit'"));
	if (TempUnit.Succeeded())
	{
		ChainUnit1->SetStaticMesh(TempUnit.Object);
		ChainUnit2->SetStaticMesh(TempUnit.Object);
		ChainUnit3->SetStaticMesh(TempUnit.Object);
		ChainUnit4->SetStaticMesh(TempUnit.Object);
		ChainUnit5->SetStaticMesh(TempUnit.Object);
		ChainUnit6->SetStaticMesh(TempUnit.Object);
		ChainUnit7->SetStaticMesh(TempUnit.Object);
		ChainUnit8->SetStaticMesh(TempUnit.Object);
	}

}

// Called when the game starts or when spawned
void AChain::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AChain::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

