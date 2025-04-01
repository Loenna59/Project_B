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

	for (int32 i = 0; i < 2; i++)
	{
		AHammer* Hammer = Cast<AHammer>(GetWorld()->SpawnActor(HammerFactory));
		Hammer->SetVisible(false);
		CacheHammers.Add(Hammer);

		AWineBottle* Bottle = Cast<AWineBottle>(GetWorld()->SpawnActor(BottleFactory));
		Bottle->SetVisible(false);
		CacheWineBottles.Add(Bottle);
	}

	LOG_SCREEN("[WeaponSpawnManager] 무기 생성 시작!");
	Spawn();
}

void AWeaponSpawnManager::Spawn()
{
	//TODO: TEST...
	CacheHammers[0]->SetActorLocation(SpawnPoints[0]->GetActorLocation());
	CacheHammers[0]->SetVisible(true);

	CacheWineBottles[0]->SetActorLocation(SpawnPoints[1]->GetActorLocation());
	CacheWineBottles[0]->SetVisible(true);
}

