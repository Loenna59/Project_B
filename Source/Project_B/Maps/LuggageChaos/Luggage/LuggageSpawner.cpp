// Fill out your copyright notice in the Description page of Project Settings.


#include "LuggageSpawner.h"

#include "Luggage.h"
#include "Project_B/Utilities/LogMacro.h"


ALuggageSpawner::ALuggageSpawner()
{
	PrimaryActorTick.bCanEverTick = true;
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

void ALuggageSpawner::BeginPlay()
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

	InitLuggagePool();
	GoldLuggage = SpawnGoldLuggage();

	//일반 러기지 타이머 루프 시작
	GetWorldTimerManager().SetTimer(NormalTimerHandle,this,&ALuggageSpawner::AcquirePooledObject,SpawnDuration,true);
	
	ActiveGoldLuggage();
}

void ALuggageSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ALuggageSpawner::InitLuggagePool()
{
	LuggagePool.Empty(PoolSize);
	for (uint8 i = 0; i < PoolSize; i++)
	{
		LuggagePool.Add(SpawnLuggage());
	}
}

void ALuggageSpawner::AcquirePooledObject()
{
	for (uint8 i = 0; i < PoolSize; i++)
	{
		if (LuggagePool[i]->IsHidden())
		{
			LuggagePool[i]->LuggageMesh->SetSimulatePhysics(true);
			LuggagePool[i]->SetActorEnableCollision(true);
			LuggagePool[i]->SetActorHiddenInGame(false);

			return ;
		}
	}

	LOG_SCREEN("We’re short on luggage. Expanding luggage pool. (0-0);;");
	ExpandObjectPool();
}

void ALuggageSpawner::ReturnPooledObject(ALuggage* lug)
{
	lug->LuggageMesh->SetSimulatePhysics(false);
	lug->SetActorEnableCollision(false);
	lug->SetActorHiddenInGame(true);
	lug->SetActorLocation(GetActorLocation());
}


void ALuggageSpawner::ExpandObjectPool()
{
	uint8 curSize = LuggagePool.Num();
	LuggagePool.SetNum(curSize + ExpandPoolSize);

	PoolSize += ExpandPoolSize;

	for (uint8 i = curSize; i < LuggagePool.Num(); i++)
	{
		LuggagePool[i] = SpawnLuggage();
	}

	AcquirePooledObject();
}

void ALuggageSpawner::ActiveGoldLuggage()
{
	GoldLuggage->LuggageMesh->SetSimulatePhysics(true);
	GoldLuggage->SetActorEnableCollision(true);
	GoldLuggage->SetActorHiddenInGame(false);

	LOG_SCREEN("황금 러기지 등장  (*ﾟuﾟ )/\"");
	
}

void ALuggageSpawner::DeactiveGoldLuggage()
{
	GoldLuggage->LuggageMesh->SetSimulatePhysics(false);
	GoldLuggage->SetActorEnableCollision(false);
	GoldLuggage->SetActorHiddenInGame(true);
	GoldLuggage->SetActorLocation(GetActorLocation());

	GetWorldTimerManager().SetTimer(GoldTimerHandle,this,&ALuggageSpawner::ActiveGoldLuggage,GoldCoolTime);
	
	LOG_SCREEN("황금 러기지 퇴장... ε=ε=ε=┏(ﾟロﾟ;)┛");
}

ALuggage* ALuggageSpawner::SpawnLuggage()
{
	ALuggage* lug = GetWorld()->SpawnActor<ALuggage>(BP_LuggageNormal, GetTransform());

	lug->LuggageMesh->SetSimulatePhysics(false);
	lug->SetActorEnableCollision(false);
	lug->SetActorHiddenInGame(true);
	
	return lug;
}

ALuggage* ALuggageSpawner::SpawnGoldLuggage()
{
	ALuggage* lug = GetWorld()->SpawnActor<ALuggage>(BP_LuggageGold, GetTransform());

	lug->LuggageMesh->SetSimulatePhysics(false);
	lug->SetActorEnableCollision(false);
	lug->SetActorHiddenInGame(true);
	
	return lug;
}
