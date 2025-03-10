// Fill out your copyright notice in the Description page of Project Settings.


#include "Project_B/Maps/BlackHole/Public/DestroyZone.h"

#include "DataTableEditorUtils.h"
#include "Components/SphereComponent.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "Project_B/Utilities/LogMacro.h"


// Sets default values
ADestroyZone::ADestroyZone()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 외관
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);
	BottomBlade = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BottomBlade"));
	BottomBlade->SetupAttachment(RootComponent);
	BottomBlade->SetRelativeLocation(FVector(0, 0, 200));
	BottomBlade->SetRelativeRotation(FRotator(0,0,-90));
	BottomRotator = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BottomRotator"));
	BottomRotator->SetupAttachment(RootComponent);
	BottomRotator->SetRelativeRotation(FRotator(0,0,90));
	DestroyZoneSphere = CreateDefaultSubobject<USphereComponent>(TEXT("DestroyZoneSphere"));
	DestroyZoneSphere->SetupAttachment(RootComponent);
	DestroyZoneSphere->SetRelativeLocation(FVector(0, 0, 100));
	DestroyZoneSphere->SetRelativeScale3D(FVector(10));

	// 회전
	BladeRotateComp = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("BladeRotateComp"));
	BladeRotateComp->SetUpdatedComponent(BottomBlade);
	BladeRotateComp->RotationRate = FRotator(RotateSpeed,0,0);
	BladeRotateComp->SetAutoActivate(false);
	
	RotateComp = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("RotateComp"));
	RotateComp->SetUpdatedComponent(BottomRotator);
	RotateComp->SetAutoActivate(false);
	RotateComp->RotationRate = FRotator(RotateSpeed,0,0);
	RotateComp->PivotTranslation = FVector(0, -1, 0);
	
	// 외관 넣어주기
	ConstructorHelpers::FObjectFinder<UStaticMesh>TempBlade(TEXT("/Script/Engine.StaticMesh'/Game/Assets/Blackhole/Blackhole_BottomBlade.Blackhole_BottomBlade'"));
	if (TempBlade.Succeeded())
	{
		BottomBlade->SetStaticMesh(TempBlade.Object);
	}
	ConstructorHelpers::FObjectFinder<UStaticMesh>TempRotator(TEXT("/Script/Engine.StaticMesh'/Game/Assets/Blackhole/Blackhole_BottomRotor.Blackhole_BottomRotor'"));
	if (TempRotator.Succeeded())
	{
		BottomRotator->SetStaticMesh(TempRotator.Object);
	}

}

void ADestroyZone::OnDestroyBeginOverlap(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	const FHitResult& SweepResult)
{
	// 플레이어 죽음
	LOG_PRINT(TEXT("플레이어 죽음!"));
}

// Called when the game starts or when spawned
void ADestroyZone::BeginPlay()
{
	Super::BeginPlay();

	DestroyZoneSphere->SetGenerateOverlapEvents(true);
	DestroyZoneSphere->OnComponentBeginOverlap.AddDynamic(this,&ADestroyZone::OnDestroyBeginOverlap);
	
}

// Called every frame
void ADestroyZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
		
}

void ADestroyZone::BladeRotate()
{
	if (bIsOnBlackhole)
	{
		BladeRotateComp->SetAutoActivate(true);
		RotateComp->SetAutoActivate(true);
		// 회전 속도 조정
	}
}

