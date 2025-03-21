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

	// 블랙홀 캐스팅
	Blackhole = Cast<ABlackHole>(UGameplayStatics::GetActorOfClass(GetWorld(), ABlackHole::StaticClass()));
	Planet = Blackhole;
	// 플레이어 캐스팅
	AActor* OwnerActor = GetOwner(); 
	PlayerCharacter = Cast<ACharacter>(OwnerActor);
	// 게임모드 캐스팅
	gm = Cast<ABlackholeGameMode>(GetWorld()->GetAuthGameMode());

	// 각 액터마다 랜덤한 초기 각도 부여
	CurrentOrbitAngle = FMath::RandRange(0.0f, 360.0f);
}


// Called every frame
void UGravityComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                      FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (Planet && Blackhole->bIsActive)
	{
		ApplyGravity(DeltaTime);
		SpawnCount();
	}
	if (false == Blackhole->bIsActive && gm->BlackholeSpawnCount != 0)
	{
		DeactivateGravity();
	}
}

void UGravityComponent::ApplyGravity(float DeltaTime)
{
	if (!OwnerPhysicsComp || !Planet) return;

	// 공용 변수
	PlanetLocation = Planet->GetActorLocation();
	ObjectLocation = GetOwner()->GetActorLocation();
	// 오브젝트->블랙홀을 바라보는 방향
	Direction = FVector(ObjectLocation - PlanetLocation).GetSafeNormal();
	// 둘 사이 거리
	Distance = FVector::Dist(PlanetLocation, ObjectLocation);

	if (PlayerCharacter)
	{
		PlayerCharacter->GetCapsuleComponent()->SetSimulatePhysics(true);
	}
	
	// 1. 자전 구현 (Yaw,Pitch 회전)
	FRotator CurrentRotation = GetOwner()->GetActorRotation();
	CurrentRotation.Yaw += RotationSpeed * 4 * DeltaTime;
	CurrentRotation.Pitch += RotationSpeed * 2 * DeltaTime ;
	GetOwner()->SetActorRotation(CurrentRotation);
	
	// 2. 블랙홀방향으로 끌어당기는 힘 (중력)
	FVector StartLoc = ObjectLocation;
	FVector EndLoc = PlanetLocation;
	FRotator InRot = UKismetMathLibrary::FindLookAtRotation(StartLoc, EndLoc);
	FVector GravityDir = UKismetMathLibrary::GetForwardVector(InRot);
	// 원심력 고려해서 중력 크기 조절 (속도가 빠를수록 중력 약화)
	float GravityScale = FMath::Clamp(1000.0f / (OrbitSpeed + 1.0f), 200.0f, 1000.0f);
	FVector GravityStrength = GravityDir * GravityScale;
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
	InterpSpeed = FMath::Clamp(OrbitSpeed * 0.02f, 0.1f, 5.0f);
	FVector SmoothedPosition = UKismetMathLibrary::VInterpTo(ObjectLocation, TargetPosition, DeltaTime, InterpSpeed);

	// 새 위치로 이동하자
	GetOwner()->SetActorLocation(SmoothedPosition);
}

void UGravityComponent::DeactivateGravity()
{
	if (!OwnerPhysicsComp || !Planet) return;
	
	// 현재 속도 가져오기
	FVector CurrentVelocity = OwnerPhysicsComp->GetPhysicsLinearVelocity();
	// X, Y축 속도를 0으로 만들고, Z축만 중력 방향으로 설정
	FVector NewVelocity = FVector(0, 0, -500.0f);
	
	if (PlayerCharacter)
	{
		PlayerCharacter->GetCapsuleComponent()->SetPhysicsLinearVelocity(NewVelocity, false, "None");
		PlayerCharacter->GetCapsuleComponent()->SetSimulatePhysics(false);
		SetComponentTickEnabled(false);
		GetWorld()->GetTimerManager().SetTimer(AlignHandle, this, &UGravityComponent::AlignCharacter, 0.1f, true);
	}
	else
	{
		// 새로운 속도 적용
		OwnerPhysicsComp->SetPhysicsLinearVelocity(NewVelocity, false, "None");
	}
}

void UGravityComponent::SpawnCount()
{
	if (gm)
	{
		// 블랙홀 페이즈별 공전궤도와 힘을 설정해주자
		switch (gm->BlackholeSpawnCount)
		{
		case 0:
			OrbitRadius = 900;
			OrbitSpeed = 10.0f;
			break;
		case 1:
			OrbitRadius = 750;
			OrbitSpeed = 40.0f;
			break;
		case 2:
			OrbitRadius = 500;
			OrbitSpeed = 70.0f;
			break;
		default:
			OrbitRadius = 400;
			OrbitSpeed = 80.0f;
			break;
		}
	}
}

void UGravityComponent::AlignCharacter()
{
	if (PlayerCharacter)
	{
		FRotator CurrentRotation = PlayerCharacter->GetActorRotation();
		FRotator TargetRotation = FRotator(0.0f, CurrentRotation.Yaw, 0.0f);

		// 부드럽게 회전 적용
		FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, GetWorld()->DeltaTimeSeconds, 1.0f);
		PlayerCharacter->SetActorRotation(NewRotation);

		// 충분히 정렬되었으면 타이머 중지
		if (FMath::Abs(NewRotation.Pitch) < 1.0f && FMath::Abs(NewRotation.Roll) < 1.0f)
		{
			GetWorld()->GetTimerManager().ClearTimer(AlignHandle);
			SetComponentTickEnabled(true);
		}
	}
}
