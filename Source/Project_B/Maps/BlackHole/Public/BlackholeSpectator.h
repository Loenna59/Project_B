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

	
	void SpawnProjectile();
	
	APlayerController* pc;
	UPROPERTY()
	class UCameraComponent* SpectatorCam;

	// 인풋
	UPROPERTY()
	class UInputMappingContext* IMC_Spectator;
	UPROPERTY()
	class UInputAction* IA_Fire;
};
