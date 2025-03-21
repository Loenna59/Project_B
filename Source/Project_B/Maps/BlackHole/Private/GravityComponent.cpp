// Fill out your copyright notice in the Description page of Project Settings.


#include "Project_B/Maps/BlackHole/Public/GravityComponent.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/GameModeBase.h"
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

	// 게임모드?
	gm = Cast<ABlackholeGameMode>(GetWorld()->GetAuthGameMode());
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
	CurrentRotation.Yaw += RotationSpeed * 4 * DeltaTime;
	CurrentRotation.Pitch += RotationSpeed * 2 * DeltaTime ;
	GetOwner()->SetActorRotation(CurrentRotation);
	
	// 2. 블랙홀방향으로 끌어당기는 힘 (중력)
	FVector StartLoc = ObjectLocation;
	FVector EndLoc = PlanetLocation;
	FRotator InRot = UKismetMathLibrary::FindLookAtRotation(StartLoc, EndLoc);
	GravityStrength = UKismetMathLibrary::GetForwardVector(InRot)*100;
	if (PlayerCharacter)
	{
		PlayerCharacter->GetCapsuleComponent()->SetPhysicsLinearVelocity(GravityStrength, false, "None");
	}
	else
	{
		OwnerPhysicsComp->SetPhysicsLinearVelocity(GravityStrength, false, "None");
	}

	// 3. 궤도 운동을 할 수 있게 하자
	// 현재 각도를 기반으로 궤도 상의 다음 위치 계산
	CurrentOrbitAngle += OrbitSpeed * DeltaTime; // 각도 증가시키고
	if (CurrentOrbitAngle > 360.0f) 
	{
		// 각도가 360도를 넘어가면 초기화
		CurrentOrbitAngle -= 360.0f;
	}

	// 궤도 상의 새 위치 계산하자
	float RadianAngle = FMath::DegreesToRadians(CurrentOrbitAngle);
	// 원형 궤도 계산
	FVector OrbitOffset = FVector(FMath::Cos(RadianAngle), FMath::Sin(RadianAngle), 0) * OrbitRadius;
	// 블랙홀 위치를 기준으로 궤도 위치 계산
	FVector TargetPosition = PlanetLocation + OrbitOffset; 

	// 선형 보간 사용
	float InterpSpeed = 1.0f;
	FVector SmoothedPosition = UKismetMathLibrary::VInterpTo(ObjectLocation, TargetPosition, DeltaTime, InterpSpeed);

	// 새 위치로 이동하자
	GetOwner()->SetActorLocation(SmoothedPosition);
}
