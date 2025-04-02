// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SpectatorPawn.h"
#include "BlackholeSpectator.generated.h"

UCLASS()
class PROJECT_B_API ABlackholeSpectator : public ASpectatorPawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABlackholeSpectator();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	// 물체 스폰 함수
	void SpawnProjectile();
	// 물체 생성은 서버 권한으로 실행되게하자~!
	// 동기화
	UFUNCTION(Server, Reliable)
	void ServerRPC_SpawnProjectile(FVector SpawnLocation, FVector TargetLocation);
	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_SpawnProjectile(FVector SpawnLocation, FVector TargetLocation);
	// 스폰
	void SpawnProjectileInternational(FVector SpawnLocation, FVector TargetLocation);

public:
	APlayerController* pc;
	UPROPERTY(EditAnywhere)
	class UCameraComponent* SpectatorCam;

	// 인풋
	UPROPERTY()
	class UInputMappingContext* IMC_Spectator;
	UPROPERTY()
	class UInputAction* IA_Fire;

	// 위젯을 띄우자
	UPROPERTY()
	TSubclassOf<class USpectatorUI> SpectatorUIFactory;
	UPROPERTY()
	class USpectatorUI* SpectatorUI;
	void CreateSpectatorUI();

	// 스폰시킬 물체
	UPROPERTY(EditAnywhere)
	TSubclassOf<class ASpectatorItem> SepctatorItmeFactory;
	UPROPERTY(EditAnywhere)
	class ASpectatorItem* SpectatorItem;
};
