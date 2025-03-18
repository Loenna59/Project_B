

#include "ConveyorBeltLong.h"

#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "Project_B/Utilities/LogMacro.h"

AConveyorBeltLong::AConveyorBeltLong()
{
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(Root);

	StartArrow = CreateDefaultSubobject<UArrowComponent>("StartArrow");
	StartArrow->SetupAttachment(Root);

	EndBox = CreateDefaultSubobject<UBoxComponent>("EndBox");
	EndBox->SetupAttachment(Root);
	EndBox->SetRelativeRotation(FRotator(0.0f,-90.0f,0.0f));
	EndBox->SetRelativeScale3D(FVector(3.500000,1.250000,2.500000));

	ForceBox = CreateDefaultSubobject<UBoxComponent>("ForceBox");
	ForceBox->SetupAttachment(Root);
	ForceBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	ForceBox->SetCollisionResponseToChannel(ECC_Pawn,ECR_Overlap);
	
	Plates.SetNum(PlateCount);
	for (int i = 0; i < PlateCount; i++)
	{
		FName PlateName = FName(*FString::Printf(TEXT("Plate_%d"), i));
		Plates[i] = CreateDefaultSubobject<UStaticMeshComponent>(PlateName);
		Plates[i]->SetupAttachment(Root);
		Plates[i]->SetRelativeLocation(FVector(i*100.0f, 0, 0));
		Plates[i]->SetRelativeRotation(FRotator(0.0f,-90.0f,0.0f));
	}
}

void AConveyorBeltLong::BeginPlay()
{
	Super::BeginPlay();
	
	for (int i = 0; i < PlateCount; i++)
	{
		Plates[i]->OnComponentEndOverlap.AddDynamic(this, &AConveyorBeltLong::OnCollisionEndOverlap);
	}

	MoveDir = StartArrow->GetComponentLocation().ForwardVector;
}

void AConveyorBeltLong::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	for (int i = 0; i < PlateCount; i++)
	{
		Plates[i]->SetRelativeLocation(Plates[i]->GetRelativeLocation() + MoveDir * DeltaTime*Speed);
	}
}

void AConveyorBeltLong::OnCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherComp == EndBox)
	{
		OverlappedComponent->SetRelativeLocation(StartArrow->GetRelativeLocation());
	}
}

void AConveyorBeltLong::OnCharacterStepOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

void AConveyorBeltLong::OnCharacterStepEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}
