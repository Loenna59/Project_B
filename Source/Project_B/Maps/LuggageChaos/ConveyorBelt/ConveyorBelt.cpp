

#include "ConveyorBelt.h"

#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"
#include "Project_B/Utilities/LogMacro.h"


AConveyorBelt::AConveyorBelt()
{

	Root = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(Root);

	StartArrow = CreateDefaultSubobject<UArrowComponent>("StartArrow");
	StartArrow->SetupAttachment(Root);
	
	Plates.SetNum(PlateCount);
	PlateInitPositions.SetNum(PlateCount);
	
	for (int i = 0; i < PlateCount; i++)
	{
		FName PlateName = FName(*FString::Printf(TEXT("Plate_%d"), i));
		Plates[i] = CreateDefaultSubobject<UStaticMeshComponent>(PlateName);
		Plates[i]->SetupAttachment(Root);
		Plates[i]->SetRelativeLocation(FVector(i*100.0f, 0, 0));
		Plates[i]->SetRelativeRotation(FRotator(0.0f,-90.0f,0.0f));
	}

	bReplicates = true;
	Super::SetReplicateMovement(true);
}

void AConveyorBelt::BeginPlay()
{
	Super::BeginPlay();
	
	for (int i = 0; i < PlateCount; i++)
	{
		PlateInitPositions[i] = Plates[i]->GetComponentLocation();
	}

	MoveDir = StartArrow->GetForwardVector();
	
	StarLoc = Plates[0]->GetComponentLocation();
	EndLoc = Plates[PlateCount -1]->GetComponentLocation();
	
	MaxDist = FVector::DotProduct(EndLoc - StarLoc, MoveDir);
}



