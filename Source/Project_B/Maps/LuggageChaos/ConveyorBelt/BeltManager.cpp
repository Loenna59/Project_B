

#include "BeltManager.h"

#include "ConveyorBelt.h"
#include "ConveyorBeltLong.h"
#include "Net/UnrealNetwork.h"
#include "Project_B/Utilities/LogMacro.h"


ABeltManager::ABeltManager()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ABeltManager::BeginPlay()
{
	Super::BeginPlay();

	bReplicates = true;

	if (HasAuthority())
	{
		FTimerHandle OnStartTimerHandle;
		GetWorld()->GetTimerManager().SetTimer(OnStartTimerHandle, this, &ABeltManager::Net_StartMoving,2.0f,false);
	}
}

void ABeltManager::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void ABeltManager::Net_StartMoving_Implementation()
{
	bIsMoving = true;
}

void ABeltManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsMoving == false)
	{
		return;
	}
	
	for (int i = 0; i < LongBelts.Num(); i++)
	{
		TArray<UStaticMeshComponent*> plates = LongBelts[i]->Plates;
		
		for (int j = 0; j < plates.Num(); j++)
		{
			FVector newPos = plates[j]->GetComponentLocation()  + LongBelts[i]->MoveDir * DeltaTime * Speed;
		
			float newDist = FVector::DotProduct(newPos - LongBelts[i]->StarLoc, LongBelts[i]->MoveDir);
		
			if (newDist >= LongBelts[i]->MaxDist)
			{
				// newPos = StarLoc + (newDist - MaxDist)*MoveDir;
				newPos = LongBelts[i]->StarLoc;
			}
		
			plates[j]->SetWorldLocation(newPos);
		}
	}

	for (int i = 0; i < ShortBelts.Num(); i++)
	{
		TArray<UStaticMeshComponent*> plates = ShortBelts[i]->Plates;
		
		for (int j = 0; j < plates.Num(); j++)
		{
			FVector newPos = plates[j]->GetComponentLocation()  + ShortBelts[i]->MoveDir * DeltaTime * Speed;
		
			float newDist = FVector::DotProduct(newPos - ShortBelts[i]->StarLoc, ShortBelts[i]->MoveDir);
		
			if (newDist >= ShortBelts[i]->MaxDist)
			{
				// newPos = StarLoc + (newDist - MaxDist)*MoveDir;
				newPos = ShortBelts[i]->StarLoc;
			}
		
			plates[j]->SetWorldLocation(newPos);
		}
	}
}

