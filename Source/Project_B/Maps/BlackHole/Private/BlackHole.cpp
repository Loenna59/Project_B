// Fill out your copyright notice in the Description page of Project Settings.


#include "Project_B/Maps/BlackHole/Public/BlackHole.h"

#include "Character/BaseCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "Project_B/Maps/BlackHole/Public/BoxAsset.h"
#include "Project_B/Maps/BlackHole/Public/Chain.h"
#include "Project_B/Maps/BlackHole/Public/GravityComponent.h"


class URadialForceComponent;
class ABoxAsset;
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
	Sphere->SetRelativeScale3D(FVector(4.5f));
	Sphere->SetVisibility(false);

	// 오버랩 필드
	SphereComp = CreateDefaultSubobject<USphereComponent>("Round");
	SphereComp->SetupAttachment(RootComponent);
	SphereComp->SetSphereRadius(1500);
	
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

void ABlackHole::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		UGravityComponent* GravityComp = OtherActor->FindComponentByClass<UGravityComponent>();
		if (GravityComp)
		{
			GravityComp->ApplyGravity(GetWorld()->DeltaTimeSeconds);
		}
	}
}

// Called when the game starts or when spawned
void ABlackHole::BeginPlay()
{
	Super::BeginPlay();

	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ABlackHole::OnOverlap);
}

// Called every frame
void ABlackHole::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 활성화 되면 빨아들이기 시작
	if (bIsActive)
	{
		Sphere->SetVisibility(true);
		ActivateBlackhole();
	}
	else
	{
		Sphere->SetVisibility(false);
	}
}

void ABlackHole::ActivateBlackhole()
{
	// 체인 조사해서 포함시키기
	AChain* ChainCable = Cast<AChain>(UGameplayStatics::GetActorOfClass(GetWorld(), AChain::StaticClass()));
	UStaticMeshComponent* HandleComp = nullptr;
	
	if (ChainCable)
	{
		TArray<UActorComponent*> ChainComponents = ChainCable->GetComponentsByTag(UStaticMeshComponent::StaticClass(), FName("Handle"));
		if (ChainComponents.Num() > 0)
		{
			HandleComp = Cast<UStaticMeshComponent>(ChainComponents[0]);
			FVector StartLoc = HandleComp->GetComponentLocation();
			FVector EndLoc = GetActorLocation();
			FRotator InRot = UKismetMathLibrary::FindLookAtRotation(StartLoc, EndLoc);
			GravityVel = UKismetMathLibrary::GetForwardVector(InRot)*300;
			
			HandleComp->SetPhysicsLinearVelocity(GravityVel, false, "None");
		}
	}
}

