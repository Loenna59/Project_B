// Fill out your copyright notice in the Description page of Project Settings.


#include "Project_B/Maps/BlackHole/Public/BlackHole.h"

#include "Character/BaseCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "Project_B/Maps/BlackHole/Public/BoxAsset.h"
#include "Project_B/Maps/BlackHole/Public/Chain.h"


class URadialForceComponent;
class ABoxAsset;
// Sets default values
ABlackHole::ABlackHole()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 외관
	Root = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(Root);
	Sphere = CreateDefaultSubobject<UStaticMeshComponent>("Sphere");
	Sphere->SetupAttachment(RootComponent);
	// 중력필드
	GravityField = CreateDefaultSubobject<URadialForceComponent>("GravityField");
	GravityField->SetupAttachment(RootComponent);
	GravityField->Radius = 0.f; // 중력 필드 반경
	GravityField->ForceStrength = 0.f; // 중력 강도 (음수는 끌어당김)
	
	ConstructorHelpers::FObjectFinder<UStaticMesh>TempBlackHole(TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
	if (TempBlackHole.Succeeded())
	{
		Sphere->SetStaticMesh(TempBlackHole.Object);
	}
	ConstructorHelpers::FObjectFinder<UMaterialInstance>TempBlackHoleMat(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Maps/Blackhole/Material/M_BlackHole_Inst.M_BlackHole_Inst'"));
	if (TempBlackHoleMat.Succeeded())
	{
		Sphere->SetMaterial(0, TempBlackHoleMat.Object);
	}
}

// Called when the game starts or when spawned
void ABlackHole::BeginPlay()
{
	Super::BeginPlay();

	// 처음에 생성될때 크기 0으로 설정했다가 점점 커지게 (4로커지면됨)
	// Sphere->SetRelativeScale3D(FVector(0));

}

// Called every frame
void ABlackHole::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 활성화 되면 빨아들이기 시작
	if (bIsActive)
	{
		// bIsActive가 처음으로 true가 될 때만 중력 필드 생성
		if (!bGravityFieldCreated)
		{
			CreateGravityField();
			bGravityFieldCreated = true; // 중력 필드가 생성되었음을 표시
		}
		ActivateBlackhole();
		ApplyOrbitalForce();
	}
	else
	{
		DeactivateBlackhole();
	}
}

void ABlackHole::SetBlackholeState(bool bNewState)
{
	if (bIsActive != bNewState)
	{
		bIsActive = bNewState;
		OnBlackholeStateChanged.Broadcast(bIsActive); // 델리게이트 호출
	}
}

void ABlackHole::CreateGravityField()
{
	GravityField->Radius = 1500.0f; // 중력 필드 반경
	GravityField->ForceStrength = 2000.0f; // 중력 강도
}

void ABlackHole::ApplyOrbitalForce()
{
	// box 전부 조사해서 배열에 저장하자
	TArray<AActor*> BoxActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABoxAsset::StaticClass(), BoxActors);

	//범위기반 for 루프, 저장된 액터를 하나씩 순회
	for (AActor* BoxActor : BoxActors)
	{
		ABoxAsset* BoxAsset = Cast<ABoxAsset>(BoxActor);
		// 메쉬 꺼내기
		UStaticMeshComponent* BoxComp = BoxAsset->Box;
		FVector BoxLocation = BoxComp->GetComponentLocation();
		FVector BlackHoleCenter = GetActorLocation();
		BoxComp->SetEnableGravity(false);
		
		// 사이의 거리값
		float Distance = FVector::Dist(BoxLocation,BlackHoleCenter);
			
		// 박스->블랙홀 방향으로 향하는 벡터 계산
		FVector DirectionToBlackHole = BlackHoleCenter - BoxLocation;
		DirectionToBlackHole.Normalize();
		
		// 블랙홀 주위를 회전하는 벡터 계산
		// 공전을 하려면 현재위치에서 블랙홀중심향하는 벡터에 수직인 방향으로 이동해야함
		// 블랙홀 방향 벡터를 Z축 기준 90도로 회전, 축을 재설정 (원형 궤도 회전할 방향임)
		FVector RotationAxis = FVector(0, 0, 1);
		FVector OrbitDirection = DirectionToBlackHole.RotateAngleAxis(90.0f, RotationAxis);

		FVector Force = OrbitDirection * OrbitPower * OrbitScale;
		// 이동할 방향으로 힘
		BoxComp->AddForce(Force, NAME_None, true);
		// 회전하면서 이동하자
		FVector TorqueForce(50, 50, 0);
		BoxComp->AddTorqueInDegrees(TorqueForce, NAME_None, true);
	}
}

void ABlackHole::DeactivateBlackhole()
{
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
	GravityField->Radius = 0.f; 
	GravityField->ForceStrength = 0.f;
}

void ABlackHole::ActivateBlackhole()
{
	// 1. 플레이어 데려오기
	ABaseCharacter* Player = Cast<ABaseCharacter>(UGameplayStatics::GetActorOfClass(GetWorld(), ABaseCharacter::StaticClass()));
	if (Player)
	{
		// 캡슐 컴포넌트
		UCapsuleComponent* capsule = Player->GetCapsuleComponent();
		capsule->SetSimulatePhysics(true);
		
		FVector StartLoc = Player->GetActorLocation();
		FVector EndLoc = Sphere->GetComponentLocation();
		FRotator InRot = UKismetMathLibrary::FindLookAtRotation(StartLoc, EndLoc);
		FVector NewVel = UKismetMathLibrary::GetForwardVector(InRot)*200;
		
		capsule->SetPhysicsLinearVelocity(NewVel, false, "None");
	}
	
	// 2. box 전부 조사해서 배열에 저장하자
	TArray<AActor*> BoxActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABoxAsset::StaticClass(), BoxActors);

	//범위기반 for 루프, 저장된 액터를 하나씩 순회
	for (AActor* BoxActor : BoxActors) 
	{
		ABoxAsset* BoxAsset = Cast<ABoxAsset>(BoxActor);
		// 메쉬 꺼내기
		UStaticMeshComponent* BoxComp = BoxAsset->Box;
		FVector StartLoc = BoxComp->GetComponentLocation();
		FVector EndLoc = Sphere->GetComponentLocation();
		FRotator InRot = UKismetMathLibrary::FindLookAtRotation(StartLoc, EndLoc);
		FVector NewVel = UKismetMathLibrary::GetForwardVector(InRot)*200;
		
		BoxComp->SetPhysicsLinearVelocity(NewVel, false, "None");
	}

	// 3. 똑같이, 체인도 조사해서 포함시키기
	AChain* ChainCable = Cast<AChain>(UGameplayStatics::GetActorOfClass(GetWorld(), AChain::StaticClass()));
	UStaticMeshComponent* HandleComp = nullptr;
	
	if (ChainCable)
	{
		TArray<UActorComponent*> ChainComponents = ChainCable->GetComponentsByTag(UStaticMeshComponent::StaticClass(), FName("Handle"));
		if (ChainComponents.Num() > 0)
		{
			HandleComp = Cast<UStaticMeshComponent>(ChainComponents[0]);
			FVector StartLoc = HandleComp->GetComponentLocation();
			FVector EndLoc = Sphere->GetComponentLocation();
			FRotator InRot = UKismetMathLibrary::FindLookAtRotation(StartLoc, EndLoc);
			FVector NewVel = UKismetMathLibrary::GetForwardVector(InRot)*200;
			
			HandleComp->SetPhysicsLinearVelocity(NewVel, false, "None");
		}
	}	
}
