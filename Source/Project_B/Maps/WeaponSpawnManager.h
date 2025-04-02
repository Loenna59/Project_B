// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponSpawnManager.generated.h"

UCLASS()
class PROJECT_B_API AWeaponSpawnManager : public AActor
{
	GENERATED_BODY()
	
public:	
	AWeaponSpawnManager();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void InitSpawn();

	UFUNCTION()
	void Disappear(EAttackType Type, int32 SpawnPointIndex);

	UFUNCTION()
	void Respawn(EAttackType Type, int32 SpawnPointIndex);

	UFUNCTION()
	void RespawnInternal(int32 SpawnPointIndex);

public:
	UPROPERTY()
	TSubclassOf<class AHammer> HammerFactory;

	UPROPERTY()
	TSubclassOf<class AWineBottle> BottleFactory;
	
	UPROPERTY()
	TArray<class AActor*> SpawnPoints;
	
	UPROPERTY()
	TArray<class AWeapon*> CacheHammers;

	UPROPERTY()
	TArray<class AWeapon*> CacheWineBottles;

	int32 WeaponIndex = 0;

	int32 VisibleHammerIndex = 0;
	
	int32 VisibleBottleIndex = 0;

	FTimerHandle RespawnHammerTimerHandle;
	
	FTimerHandle RespawnBottleTimerHandle;

};
