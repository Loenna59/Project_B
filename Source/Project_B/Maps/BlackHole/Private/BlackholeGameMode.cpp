// Fill out your copyright notice in the Description page of Project Settings.


#include "Project_B/Maps/BlackHole/Public/BlackholeGameMode.h"

#include "Character/BaseCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "Project_B/Maps/BlackHole/Public/BlackHole.h"
#include "Project_B/Maps/BlackHole/Public/BoxAsset.h"
#include "Project_B/Maps/BlackHole/Public/DestroyZone.h"


class UCapsuleComponent;
class ABaseCharacter;

ABlackholeGameMode::ABlackholeGameMode()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ABlackholeGameMode::BeginPlay()
{
	Super::BeginPlay();

	// 게임 종료 타이머 설정
	GetWorld()->GetTimerManager().SetTimer(GameTimerHandle, this, &ABlackholeGameMode::EndGame, 180.0f, false);
	
}

void ABlackholeGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABlackholeGameMode::EndGame()
{
	UE_LOG(LogTemp, Warning, TEXT("시간초과, 게임 종료!"));
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("시간 초과, 게임 종료!"));
}

