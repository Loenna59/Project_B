

#include "ConveyorBeltLong.h"

#include "Components/ArrowComponent.h"
#include "Net/UnrealNetwork.h"
#include "Project_B/Utilities/LogMacro.h"

AConveyorBeltLong::AConveyorBeltLong()
{
	PrimaryActorTick.bCanEverTick = true;

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

void AConveyorBeltLong::BeginPlay()
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

	//LOG_PRINT(TEXT("앞방향: %s"), *MoveDir.ToString());
	//LOG_PRINT(TEXT("시작위치: %s"), *StarLoc.ToString());
	//LOG_PRINT(TEXT("끝위치: %s"), *EndLoc.ToString());
	//LOG_PRINT(TEXT("이동 가능한 거리: %f"), MaxDist);
}
