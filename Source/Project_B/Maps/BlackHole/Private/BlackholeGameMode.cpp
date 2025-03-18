// Fill out your copyright notice in the Description page of Project Settings.


#include "Project_B/Maps/BlackHole/Public/BlackholeGameMode.h"

#include "Character/BaseCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "Project_B/Maps/BlackHole/Public/BlackHole.h"
#include "Project_B/Maps/BlackHole/Public/BoxAsset.h"


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

	// 블랙홀 생성 타이머 설정
	// 게임 시작 30초후 첫번째 블랙홀을 소환한다
	// TODO: 실제 시연때는 30초로 변경하기
	GetWorld()->GetTimerManager().SetTimer(BlackholeSpawnHandle, this, &ABlackholeGameMode::SpawnBlackhole, 5.0f, false);
}

void ABlackholeGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABlackholeGameMode::EndGame()
{
	UE_LOG(LogTemp, Warning, TEXT("게임 종료!"));
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("게임 종료!"));
}


inline void ABlackholeGameMode::SpawnBlackhole()
{
	// 블랙홀 스폰 함수
	// 4페이즈까지만 있다
	if(BlackholeSpawnCount >=4) return;

	FTransform SpawnTransform(FVector(0,0,600));
	Blackhole = GetWorld()->SpawnActorDeferred<ABlackHole>(ABlackHole::StaticClass(), SpawnTransform);
	if (Blackhole)
	{
		// 블랙홀 페이즈별 공전궤도와 힘을 설정해주자
		switch (BlackholeSpawnCount)
		{
		case 0:
			Blackhole->R = 1100.f;
			Blackhole->OrbitScale = 1.0f;
			Blackhole->OrbitSpeed = 20.f;
			break;
		case 1:
			Blackhole->R = 850.f;
			Blackhole->OrbitScale = 1.5f;
			Blackhole->OrbitSpeed = 40.f;
			break;
		case 2:
			Blackhole->R = 600.f;
			Blackhole->OrbitScale = 2.0f;
			Blackhole->OrbitSpeed = 60.f;
			break;
		default:
			Blackhole->R = 550.f;
			Blackhole->OrbitScale = 2.5f;
			Blackhole->OrbitSpeed = 80.f;
			break;
		}
		
		Blackhole->FinishSpawning(SpawnTransform);
		Blackhole->bIsActive = true;
	}

	// 10초 후 블랙홀 소멸
	GetWorld()->GetTimerManager().SetTimer(BlackholeDestroyHandle, this, &ABlackholeGameMode::DestroyBalckhole, 10.0f, false);
}

void ABlackholeGameMode::DestroyBalckhole()
{
	// 블랙홀 제거 함수
	if (Blackhole)
	{
		Blackhole->bIsActive = false;
		// 플레이어의 캡슐 물리 일단 꺼주기
		ABaseCharacter* Player = Cast<ABaseCharacter>(UGameplayStatics::GetActorOfClass(GetWorld(), ABaseCharacter::StaticClass()));
		UCapsuleComponent* capsule = Player->GetCapsuleComponent();
		capsule->SetSimulatePhysics(false);

		// box 전부 조사해서 배열에 저장하자
		TArray<AActor*> BoxActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABoxAsset::StaticClass(), BoxActors);

		for (AActor* BoxActor : BoxActors)
		{
			ABoxAsset* BoxAsset = Cast<ABoxAsset>(BoxActor);
			// 메쉬 꺼내기
			UStaticMeshComponent* BoxComp = BoxAsset->Box;
			BoxComp->SetEnableGravity(true);
		}
		
		// 중력필드 초기화
		Blackhole->GravityField->Radius = 0.f;
		Blackhole->GravityField->ForceStrength = 0.f;
		
		Blackhole->Destroy();
		Blackhole = nullptr;
	}
	BlackholeSpawnCount++;

	// 재소환 예약
	if (BlackholeSpawnCount < 4)
	{
		GetWorld()->GetTimerManager().SetTimer(BlackholeSpawnHandle, this, &ABlackholeGameMode::SpawnBlackhole, 10.0f, false);
	}
}
