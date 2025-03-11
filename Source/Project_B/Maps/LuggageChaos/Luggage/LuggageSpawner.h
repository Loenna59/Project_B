// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LuggageSpawner.generated.h"

UCLASS()
class PROJECT_B_API ALuggageSpawner : public AActor
{
	GENERATED_BODY()

public:
	ALuggageSpawner();

	UPROPERTY(EditAnywhere, Category = "Luggage | Normal")
	TSubclassOf<class ALuggage> BP_LuggageNormal;
	
	UPROPERTY(EditAnywhere, Category = "Luggage | Gold")
	TSubclassOf<class ALuggage> BP_LuggageGold;

	UPROPERTY(EditAnywhere, Category = "Luggage | Gold")
	float GoldCoolTime = 5.0f;
	
	UPROPERTY(EditAnywhere, Category = ObjectPool)
	uint8 PoolSize = 30;
	UPROPERTY(EditAnywhere, Category = ObjectPool)
	uint8 ExpandPoolSize = 10;
	UPROPERTY(EditAnywhere, Category = ObjectPool)
	float SpawnDuration = 5.0f;
	
	
private:
	FTimerHandle NormalTimerHandle;
	FTimerHandle GoldTimerHandle;

	UPROPERTY()
	TArray<ALuggage*> LuggagePool;

	ALuggage* GoldLuggage;

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

private:
	//일반 러기지 오브젝트 풀
	void InitLuggagePool();
	void ExpandObjectPool();

	void AcquirePooledObject();
	void ReturnPooledObject(ALuggage* lug);
	
	//골드 러기지
	void ActiveGoldLuggage();
	void DeactiveGoldLuggage();

	ALuggage* SpawnLuggage();
	ALuggage* SpawnGoldLuggage();
};
