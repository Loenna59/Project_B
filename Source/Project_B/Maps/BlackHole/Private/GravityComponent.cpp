// Fill out your copyright notice in the Description page of Project Settings.


#include "Project_B/Maps/BlackHole/Public/GravityComponent.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Project_B/Maps/BlackHole/Public/BlackHole.h"


// Sets default values for this component's properties
UGravityComponent::UGravityComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UGravityComponent::BeginPlay()
{
	Super::BeginPlay();

	// 소유자의 물리 컴포넌트 가져오기
	OwnerPhysicsComp = Cast<UPrimitiveComponent>(GetOwner()->GetComponentByClass(UPrimitiveComponent::StaticClass()));
	Blackhole = Cast<ABlackHole>(UGameplayStatics::GetActorOfClass(GetWorld(), ABlackHole::StaticClass()));
	Planet = Blackhole;
}


// Called every frame
void UGravityComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                      FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (Planet && Blackhole->bIsActive)
	{
		ApplyGravity(DeltaTime);
	}
}

void UGravityComponent::ApplyGravity(float DeltaTime)
{
	if (!OwnerPhysicsComp || !Planet) return;
	// TODO: 질량 비례 (클램프해줄것)
	// 캐릭터 클래스라면 (플레이어)
	AActor* OwnerActor = GetOwner(); 
	ACharacter* PlayerCharacter = Cast<ACharacter>(OwnerActor);
	if (PlayerCharacter)
	{
		PlayerCharacter->GetCapsuleComponent()->SetSimulatePhysics(true);
	}
	
	// 공용값
	FVector PlanetLocation = Planet->GetActorLocation();
	FVector ObjectLocation = GetOwner()->GetActorLocation();
	// 오브젝트->블랙홀을 바라보는 방향
	FVector Direction = (PlanetLocation - ObjectLocation).GetSafeNormal();
	// 둘 사이 거리
	float Distance = FVector::Dist(ObjectLocation,PlanetLocation);

	// 1. 자전 구현 (Yaw,Pitch 회전)
	FRotator CurrentRotation = GetOwner()->GetActorRotation();
	CurrentRotation.Yaw += RotationSpeed * DeltaTime * CharacterPower;
	CurrentRotation.Pitch += RotationSpeed/2 * DeltaTime * CharacterPower;
	GetOwner()->SetActorRotation(CurrentRotation);
	
	// 2. 블랙홀방향으로 끌어당기는 힘 (중력)
	FVector StartLoc = ObjectLocation;
	FVector EndLoc = PlanetLocation;
	FRotator InRot = UKismetMathLibrary::FindLookAtRotation(StartLoc, EndLoc);
	FVector NewVel = UKismetMathLibrary::GetForwardVector(InRot)*200;
	if (PlayerCharacter)
	{
		PlayerCharacter->GetCapsuleComponent()->SetPhysicsLinearVelocity(NewVel, false, "None");
	}
	else
	{
		OwnerPhysicsComp->SetPhysicsLinearVelocity(NewVel, false, "None");
	}

	// 3. 궤도 운동을 할 수 있게 하자
	FVector RotationAxis = FVector(0, 0, 1);
	FVector OrbitDirection = Direction.RotateAngleAxis(90.0f, RotationAxis);
		
	// 새 위치 계산 (회전방향으로 직진하지 않고 원형으로 회전할 수 있게)
	FVector NewPosition = ObjectLocation + (OrbitDirection * OrbitSpeed * DeltaTime);
		
	// 새위치에서 블랙홀로 향하는 방향을 구하고 정규화, 새위치 업데이트
	FVector DirectionFromBlackHole = NewPosition - PlanetLocation;
	DirectionFromBlackHole.Normalize();
	NewPosition = PlanetLocation + (DirectionFromBlackHole * Distance * CharacterPower);
        
	// 새 위치로 이동
	GetOwner()->SetActorLocation(NewPosition);
}
