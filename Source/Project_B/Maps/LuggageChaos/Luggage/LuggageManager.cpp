// Fill out your copyright notice in the Description page of Project Settings.


#include "LuggageManager.h"

#include "Luggage.h"
#include "LuggageSpawnPoint.h"
#include "Kismet/GameplayStatics.h"
#include "Project_B/Utilities/LogMacro.h"


ALuggageManager::ALuggageManager()
{
	PrimaryActorTick.bCanEverTick = true;
	
	OnReturnPooledObject.AddDynamic(this,&ALuggageManager::DeactiveLuggage);
	
	ConstructorHelpers::FClassFinder<ALuggage>luggageN(TEXT("'/Game/Maps/LuggageChaos/Luggage/BP_LuggageN.BP_LuggageN_C'"));
	if (luggageN.Succeeded())
	{
		BP_LuggageNormal = luggageN.Class;
	}
	
	ConstructorHelpers::FClassFinder<ALuggage>luggageG(TEXT("'/Game/Maps/LuggageChaos/Luggage/BP_LuggageG.BP_LuggageG_C'"));
	if (luggageG.Succeeded())
	{
		BP_LuggageGold = luggageG.Class;
	}
}

void ALuggageManager::BeginPlay()
{
	Super::BeginPlay();
	
	if (BP_LuggageNormal == nullptr)
	{
		LOG_ERROR(this,TEXT("BP_LuggageN is NULL"));
	}
	if (BP_LuggageGold == nullptr)
	{
		LOG_ERROR(this,TEXT("BP_LuggageG is NULL"));
	}
	if (BlueSpawnPoint == nullptr)
	{
		LOG_ERROR(this,TEXT("BlueSpawnPoint is NULL"));
	}
	if (RedSpawnPoint == nullptr)
	{
		LOG_ERROR(this,TEXT("RedSpawnPoint is NULL"));
	}

	InitTeamSpawnPoints();
	InitLuggagePool();
	
	GoldLuggage = SpawnGoldLuggage();
	ActiveGoldLuggage();
}

void ALuggageManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ALuggageManager::InitTeamSpawnPoints()
{
	TArray<AActor*> actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ALuggageSpawnPoint::StaticClass(), actors);

	for (AActor* actor : actors)
	{
		ALuggageSpawnPoint* point = Cast<ALuggageSpawnPoint>(actor);
		if (point->Team == ETeamType::Blue)
		{
			BlueSpawnPoint = point;
		}
		else
		{
			RedSpawnPoint = point;
		}
	}

	if (BlueSpawnPoint == nullptr || RedSpawnPoint == nullptr)
	{
		LOG_ERROR(this,TEXT("BlueSpawnPoint or RedSpawnPoint is NULL"));
	}
}

void ALuggageManager::InitLuggagePool()
{
	if (SpawnPointRelativeList.Num() == 0)
	{
		LOG_ERROR(this,TEXT("러기지 스폰 포인트 없음"));
		return;
	}

	for (uint8 i = 0; i < SpawnPointRelativeList.Num(); i++)
	{
		LuggagePool.Add(SpawnLuggage(SpawnPointRelativeList[i]));
	}
}

void ALuggageManager::ActiveLuggage(ALuggage* lug)
{
	lug->LuggageMesh->SetSimulatePhysics(true);
	lug->SetActorEnableCollision(true);
	lug->SetActorHiddenInGame(false);
}

void ALuggageManager::DeactiveLuggage(ALuggage* lug, ETeamType team)
{
	lug->LuggageMesh->SetSimulatePhysics(false);
	lug->SetActorEnableCollision(false);
	lug->SetActorHiddenInGame(true);

	//메시 컴포넌트의 물리가 활성화되어있으면 부모액터에 붙지 않고 별도로 동작함.
	//때문에 액터 위치를 변경하기 전에, 컴포넌트를 부모 액터에 다시 붙이고 위치값도 초기화해줘야 함.
	lug->LuggageMesh->AttachToComponent(lug->GetRootComponent(),FAttachmentTransformRules::KeepWorldTransform);
	lug->LuggageMesh->SetRelativeLocation(FVector::ZeroVector);

	if (lug == GoldLuggage)
	{
		GoldLuggage->SetActorLocation(GetActorLocation());

		GetWorldTimerManager().SetTimer(GoldTimerHandle,this,&ALuggageManager::ActiveGoldLuggage,GoldCoolTime);
	
		//LOG_SCREEN("황금 러기지 퇴장... ε=ε=ε=┏(ﾟロﾟ;)┛");
		return;
	}
	
	if (team == ETeamType::Blue)
	{
		//LOG_SCREEN("블루팀 골대");
		lug->SetActorLocation(BlueSpawnPoint->GetActorLocation());
	}
	else
	{
		//LOG_SCREEN("레드팀 골대");
		lug->SetActorLocation(RedSpawnPoint->GetActorLocation());
	}
	
	ActiveLuggage(lug);
}

void ALuggageManager::ActiveGoldLuggage()
{
	ActiveLuggage(GoldLuggage);
	//LOG_SCREEN("황금 러기지 등장  (*ﾟuﾟ )/\"");
}

ALuggage* ALuggageManager::SpawnLuggage(FVector pos)
{
	
	ALuggage* lug = GetWorld()->SpawnActor<ALuggage>(BP_LuggageNormal, pos,GetActorRotation());

	lug->LuggageMesh->SetSimulatePhysics(true);
	lug->SetActorEnableCollision(true);
	lug->SetActorHiddenInGame(false);
	
	return lug;
}

ALuggage* ALuggageManager::SpawnGoldLuggage()
{
	ALuggage* lug = GetWorld()->SpawnActor<ALuggage>(BP_LuggageGold, GetTransform());

	lug->LuggageMesh->SetSimulatePhysics(false);
	lug->SetActorEnableCollision(false);
	lug->SetActorHiddenInGame(true);
	
	return lug;
}
