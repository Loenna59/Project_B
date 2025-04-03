#include "WeaponSpawnManager.h"

#include "WeaponSpawnPoint.h"
#include "Kismet/GameplayStatics.h"
#include "Project_B/Utilities/LogMacro.h"
#include "Weapon/Hammer.h"
#include "Weapon/WineBottle.h"

AWeaponSpawnManager::AWeaponSpawnManager()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	ConstructorHelpers::FClassFinder<AHammer> temp_hammer(TEXT("/Game/Blueprints/BP_Hammer.BP_Hammer_C"));

	if (temp_hammer.Succeeded())
	{
		HammerFactory = temp_hammer.Class;
	}

	ConstructorHelpers::FClassFinder<AWineBottle> temp_bottle(TEXT("/Game/Blueprints/BP_WineBottle.BP_WineBottle_C"));

	if (temp_bottle.Succeeded())
	{
		BottleFactory = temp_bottle.Class;
	}
}

void AWeaponSpawnManager::BeginPlay()
{
	Super::BeginPlay();

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AWeaponSpawnPoint::StaticClass(), SpawnPoints);

	WeaponIndex = FMath::RandRange(0, 1);
	
	for (int32 i = 0; i < 2; i++)
	{
		AHammer* Hammer = Cast<AHammer>(GetWorld()->SpawnActor(HammerFactory));
		Hammer->OnRequestRespawn.BindUObject(this, &AWeaponSpawnManager::Disappear);
		Hammer->SetVisible(false);
		CacheHammers.Add(Hammer);

		AWineBottle* Bottle = Cast<AWineBottle>(GetWorld()->SpawnActor(BottleFactory));
		Bottle->OnRequestRespawn.BindUObject(this, &AWeaponSpawnManager::Disappear);
		Bottle->SetVisible(false);
		CacheWineBottles.Add(Bottle);
	}

	//LOG_SCREEN("[WeaponSpawnManager] 무기 생성 시작!");
	InitSpawn();
}

void AWeaponSpawnManager::InitSpawn()
{
	int32 HammerIndex = FMath::RandRange(0, 1);
	int32 BottleIndex = (HammerIndex + 1) % 2;

	CacheHammers[0]->SetActorLocation(SpawnPoints[HammerIndex]->GetActorLocation());
	CacheHammers[0]->SetVisible(true, HammerIndex);

	CacheWineBottles[0]->SetActorLocation(SpawnPoints[BottleIndex]->GetActorLocation());
	CacheWineBottles[0]->SetVisible(true, BottleIndex);
}

void AWeaponSpawnManager::Disappear(EAttackType Type, int32 SpawnPointIndex)
{
	switch (Type)
	{
	case EAttackType::HAMMER:
		{
			++VisibleHammerIndex;
			VisibleHammerIndex %= 2;
		}
		break;
	case EAttackType::BOTTLE:
		{
			++VisibleBottleIndex;
			VisibleBottleIndex %= 2;
		}
		break;
	default:
		break;
	}

	//LOG_SCREEN("%d", SpawnPointIndex);

	Respawn(Type, SpawnPointIndex);
}

void AWeaponSpawnManager::Respawn(EAttackType Type, int32 SpawnPointIndex)
{
	TWeakObjectPtr<AWeaponSpawnManager> WeakThis = this;

	switch (Type)
	{
	case EAttackType::HAMMER:
	{
		GetWorld()->GetTimerManager()
		.SetTimer
		(
			RespawnHammerTimerHandle,
			[WeakThis, SpawnPointIndex]()
			{
				if (WeakThis.IsValid())
				{
					WeakThis->RespawnInternal(SpawnPointIndex);
				}
			},
			5.f,
			false
		);
		break;
	}
	case EAttackType::BOTTLE:
	{
		GetWorld()->GetTimerManager()
		.SetTimer
		(
			RespawnBottleTimerHandle,
			[WeakThis, SpawnPointIndex]()
			{
				if (WeakThis.IsValid())
				{
					WeakThis->RespawnInternal(SpawnPointIndex);
				}
			},
			5.f,
			false
		);
		break;
	}
	default:
		break;
	}
	
}

void AWeaponSpawnManager::RespawnInternal(int32 SpawnPointIndex)
{
	FVector SpawnLocation = SpawnPoints[SpawnPointIndex]->GetActorLocation();

	int32 Rand = FMath::RandRange(0, 1);

	if (Rand == 0)
	{
		CacheHammers[VisibleHammerIndex]->SetActorLocation(SpawnLocation);
		CacheHammers[VisibleHammerIndex]->SetVisible(true, SpawnPointIndex);
	}
	else
	{
		CacheWineBottles[VisibleBottleIndex]->SetActorLocation(SpawnLocation);
		CacheWineBottles[VisibleBottleIndex]->SetVisible(true, SpawnPointIndex);
	}
}

