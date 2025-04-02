// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Project_B/Maps/BanimalsType.h"
#include "LuggageManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnReturnPooledObject, ALuggage*, Luggage, ETeamType, Team);

class ALuggageSpawnPoint;

UCLASS()
class PROJECT_B_API ALuggageManager : public AActor
{
	GENERATED_BODY()

public:
	ALuggageManager();

	UPROPERTY(EditAnywhere, Category = "Luggage | Normal")
	TSubclassOf<class ALuggage> BP_LuggageNormal;
	
	UPROPERTY(EditAnywhere, Category = "Luggage | Gold")
	TSubclassOf<class ALuggage> BP_LuggageGold;

	UPROPERTY(EditAnywhere, Category = "Luggage | Gold")
	float GoldCoolTime = 5.0f;

	UPROPERTY(EditAnywhere, Category = "Respawn")
	float RespawnForce = 500.0f;
	
	//이벤트 델리게이트
	FOnReturnPooledObject OnReturnPooledObject;
	
private:
	FTimerHandle NormalTimerHandle;
	FTimerHandle GoldTimerHandle;
	
	UPROPERTY(EditInstanceOnly, Category = "Options|SpawnPoint", meta = (AllowPrivateAccess = "true", MakeEditWidget = true))
	TArray<FVector> SpawnPointRelativeList;

	UPROPERTY()
	ALuggageSpawnPoint* RedSpawnPoint;
	UPROPERTY()
	ALuggageSpawnPoint* BlueSpawnPoint;
	
	UPROPERTY(Replicated)
	TArray<ALuggage*> LuggagePool;

	UPROPERTY(Replicated)
	ALuggage* GoldLuggage;

protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

public:
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void ResetLuggage(ALuggage* lug, FVector loc);

private:
	/**팀별 스폰포인트를 초기화*/
	void InitTeamSpawnPoints();
	
	void InitLuggagePool();

	void ActiveLuggage(ALuggage* lug, FVector dir);
	
	void ActiveGoldLuggage();
	
	void DeactiveLuggage(ALuggage* lug, ETeamType team);

	ALuggage* SpawnLuggage(FVector pos);
	ALuggage* SpawnGoldLuggage();
};
