// Fill out your copyright notice in the Description page of Project Settings.


#include "Project_B/Maps/BlackHole/Public/BoxAsset.h"

#include "GameFramework/RotatingMovementComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ABoxAsset::ABoxAsset()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Box = CreateDefaultSubobject<UStaticMeshComponent>("Box");
	SetRootComponent(Box);
	Box->SetSimulatePhysics(true);
}

// Called when the game starts or when spawned
void ABoxAsset::BeginPlay()
{
	Super::BeginPlay();

	// 블랙홀 넣어주자
	BlackHole = Cast<ABlackHole>(UGameplayStatics::GetActorOfClass(GetWorld(), ABlackHole::StaticClass()));
}

// Called every frame
void ABoxAsset::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 블랙홀이 활성화 되면
	if (BlackHole && BlackHole->bIsActive)
	{
		// 블랙홀 중심에서부터의 거리계산하자
		FVector BoxLocation = GetActorLocation();
		FVector BlackHoleCenter = BlackHole->GetActorLocation();
		// 일정거리 범위내로 들어오면
		float Distance = FVector::Dist(BoxLocation,BlackHoleCenter);
		
		// 블랙홀을 중심으로 그 거리에서 회전하자
		if (Distance <= 600.f)
		{
			Box->SetSimulatePhysics(false); //멈추자
			
			// 박스->블랙홀 방향으로 향하는 벡터 계산
			FVector DirectionToBlackHole = BlackHoleCenter - BoxLocation;
			DirectionToBlackHole.Normalize();
			
			// 블랙홀 주위를 회전하는 벡터 계산
			// 공전을 하려면 현재위치에서 블랙홀중심향하는 벡터에 수직인 방향으로 이동해야함
			// 블랙홀 방향 벡터를 Z축 기준 90도로 회전, 축을 재설정 (원형 궤도 회전할 방향임)
			FVector RotationAxis = FVector(0, 0, 1);
			FVector OrbitDirection = DirectionToBlackHole.RotateAngleAxis(90.0f, RotationAxis);

			// 새 위치 계산 (회전방향으로 직진하지 않고 원형으로 회전할 수 있게 거리유지)
			FVector NewPosition = BoxLocation + (OrbitDirection * RotateSpeed * DeltaTime);
            
			// 블랙홀 중심과 거리 유지
			// 새위치에서 블랙홀로 향하는 방향을 구하고 정규화, 새위치 업데이트
			FVector DirectionFromBlackHole = NewPosition - BlackHoleCenter;
			DirectionFromBlackHole.Normalize();
			NewPosition = BlackHoleCenter + (DirectionFromBlackHole * Distance);
            
			// 새 위치로 이동
			SetActorLocation(NewPosition);
		}
	}

	// 블랙홀 활성화가 끝나면, 떨어지자
	Box->SetSimulatePhysics(true);
}

