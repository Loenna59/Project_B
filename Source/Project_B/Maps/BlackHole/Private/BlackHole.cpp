// Fill out your copyright notice in the Description page of Project Settings.


#include "Project_B/Maps/BlackHole/Public/BlackHole.h"

#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"


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
	ConstructorHelpers::FObjectFinder<UMaterialInstance>TempBlackHoleMat(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Maps/Blackhole/Material/M_BlackHole_Inst.M_BlackHole_Inst'"));
	if (TempBlackHoleMat.Succeeded())
	{
		Sphere->SetMaterial(0, TempBlackHoleMat.Object);
	}
	
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

	// box 전부 조사해서 배열에 저장하자
	TArray<AActor*> BoxActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABoxAsset::StaticClass(), BoxActors);

	//범위기반 for 루프, 저장된 액터를 하나씩 순회
	for (AActor* BoxActor : BoxActors) 
	{
		ABoxAsset* BoxAsset = Cast<ABoxAsset>(BoxActor);
		// 메쉬 꺼내기
		UStaticMeshComponent* BoxComp = BoxAsset->Box;
		FVector StartLoc = BoxComp->GetComponentLocation();
		FVector EndLoc = Sphere->GetComponentLocation();
		FRotator InRot = UKismetMathLibrary::FindLookAtRotation(StartLoc, EndLoc);
		FVector NewVel = UKismetMathLibrary::GetForwardVector(InRot)*200;
		
		BoxComp->SetPhysicsLinearVelocity(NewVel, false, "None");
	}
}

