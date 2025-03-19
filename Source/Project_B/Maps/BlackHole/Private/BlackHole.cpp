// Fill out your copyright notice in the Description page of Project Settings.


#include "Project_B/Maps/BlackHole/Public/BlackHole.h"

#include "Character/BaseCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
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
	Sphere->SetRelativeScale3D(FVector(4.5f));
	Sphere->SetVisibility(false);
	
	// 중력필드
	GravityField = CreateDefaultSubobject<URadialForceComponent>("GravityField");
	GravityField->SetupAttachment(RootComponent);
	GravityField->Radius = 0.f; // 중력 필드 반경
	GravityField->ForceStrength = 0.f; // 중력 강도 (음수는 끌어당김)
	GravityField->bImpulseVelChange = false;
	GravityField->bIgnoreOwningActor = true;
	
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

	Player = Cast<ABaseCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
}

// Called every frame
void ABlackHole::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 활성화 되면 빨아들이기 시작
	if (bIsActive)
	{
		Sphere->SetVisibility(true);
		// bIsActive가 처음으로 true가 될 때만 중력 필드 생성
		if (!bGravityFieldCreated)
		{
			CreateGravityField();
			bGravityFieldCreated = true; // 중력 필드가 생성되었음을 표시
		}
		ActivateGravity();
		// ActivateBlackhole();
		// ApplyOrbitalForce();
	}
	else
		Sphere->SetVisibility(false);
		// DeactiveBlackhole();
}

void ABlackHole::CreateGravityField()
{
	GravityField->Radius = 2500.0f; // 중력 필드 반경
	GravityField->ForceStrength = -50000.f; // 중력 강도
}

void ABlackHole::ActivateGravity()
{
    if (Player)
    {
        // 블랙홀 중심 -> 플레이어 사이 거리 계산
        FVector BlackholeLocation = GetActorLocation();
        FVector PlayerLocation = Player->GetActorLocation();
        float Distance = FVector::Dist(PlayerLocation, BlackholeLocation);

        // 중력 (구심력)
        float GM = FMath::Abs(GravityField->ForceStrength);
        float m = (Player->GetMesh()->GetMass()); // 플레이어의 질량
        float GravityForce = (GM * m) / (Distance * Distance);
		float ForceClamp = FMath::Clamp(GravityForce, 0, 1.0f);

        // 원심력 
        float v = 10; //플레이어 속도값(임의)
        float CentrifugalForce = (m * v * v) / Distance;

        // 힘 작용 방향 (플레이어 -> 블랙홀로 향하는 방향)
        FVector GravityDirection = (BlackholeLocation - PlayerLocation).GetSafeNormal();
    	
    	// 높이 700 미만인 경우: 나선 운동
    	if (PlayerLocation.Z < 600.0f)
    	{
    		// 700까지는 플레이어를 위로 띄우자
    		Player->GetCharacterMovement()->AddForce(GravityDirection * ForceClamp * 130000 * 2);

    		// 접선 방향으로 힘 적용 (나선 운동)
    		FVector OrbitalDirection = FVector(-GravityDirection.Y, GravityDirection.X, 0).GetSafeNormal(); // 접선 방향
    		float SpiralForce = 500.0f; // 나선 운동을 위한 힘 (조정 가능)
    		Player->GetCharacterMovement()->AddForce(OrbitalDirection * SpiralForce * 5000);

    		DrawDebugLine(GetWorld(), PlayerLocation, PlayerLocation + GravityDirection * 200.0f, FColor::Red, false, -1, 0, 2.0f);
    		DrawDebugLine(GetWorld(), PlayerLocation, PlayerLocation + OrbitalDirection * 200.0f, FColor::Blue, false, -1, 0, 2.0f);
    	}
    	// 높이 700 이상인 경우: 궤도 운동
    	else 
    	{
    		/*// Z축 높이 고정
    		PlayerLocation.Z = 600.0f;
    		Player->SetActorLocation(PlayerLocation);*/

    		// 궤도 반지름(R)을 기준으로 힘 조정
    		FVector HorizontalDirection = GravityDirection;
    		HorizontalDirection.Z = 0; // Z축 힘 제거

    		if (Distance > R)
    		{
    			// 플레이어를 블랙홀 쪽으로 당김
    			Player->GetCharacterMovement()->AddForce(HorizontalDirection * ForceClamp * 8000000 * 2);
    			UE_LOG(LogTemp, Warning, TEXT("블랙홀쪽으로 당김:%f"), ForceClamp);
    		}
    		else if (Distance < R)
    		{
    			// 플레이어를 밀어냄
    			Player->GetCharacterMovement()->AddForce(-HorizontalDirection * ForceClamp * 8000);
    			UE_LOG(LogTemp, Warning, TEXT("플레이어를 밀어냄:%f"), ForceClamp);
    		}

    		// 궤도 속도(원심력=구심력일때 궤도를 돌기 위한 속도) 계산 및 적용
    		float OrbitalVelocity = FMath::Sqrt(GM / R);
    		
    		FVector OrbitalDirection = GravityDirection.RotateAngleAxis(90.0f, FVector(0, 0, 1));
    		FVector DesiredVelocity = OrbitalDirection * OrbitalVelocity;
    		DrawDebugLine(GetWorld(), PlayerLocation, PlayerLocation + OrbitalDirection * 800.0f, FColor::Red, false, 5, 0, 2.0f);

    		// 속도 조정
    		FVector VelocityAdjustment = (DesiredVelocity) * m * 0.01;
    		Player->GetCharacterMovement()->AddForce(VelocityAdjustment);
    		UE_LOG(LogTemp, Warning, TEXT("궤도속도조정:%f"), OrbitalVelocity);
    	}
    }
}

void ABlackHole::ActivateOrbital()
{
	if (Player)
	{
		// 블랙홀 중심 -> 플레이어 사이 거리 계산
		FVector BlackholeLocation = GetActorLocation();
		FVector PlayerLocation = Player->GetActorLocation();
		float Distance = FVector::Dist(PlayerLocation, BlackholeLocation);

		// 힘 작용 방향 (플레이어 -> 블랙홀로 향하는 방향)
		FVector GravityDirection = (BlackholeLocation - PlayerLocation).GetSafeNormal();

		// 높이 700 미만인 경우: 나선 운동
		if (PlayerLocation.Z < 700.0f)
		{
			// 접선 방향으로 힘 적용 (나선 운동)
			FVector OrbitalDirection = FVector(-GravityDirection.Y, GravityDirection.X, 0).GetSafeNormal(); // 접선 방향
			float SpiralForce = 500.0f; // 나선 운동을 위한 힘 (조정 가능)
			Player->GetCharacterMovement()->AddForce(OrbitalDirection * SpiralForce);

			DrawDebugLine(GetWorld(), PlayerLocation, PlayerLocation + GravityDirection * 100.0f, FColor::Red, false, -1, 0, 2.0f);
			DrawDebugLine(GetWorld(), PlayerLocation, PlayerLocation + OrbitalDirection * 100.0f, FColor::Blue, false, -1, 0, 2.0f);
		}
	}
}
	
void ABlackHole::ActivateBlackhole()
{
	FVector BlackHoleCenter = GetActorLocation();
	// 시간에 따른 각도 증가
	CurrentAngle += OrbitSpeed * GetWorld()->GetDeltaSeconds();
	
	// box 전부 조사해서 배열에 저장하자
	TArray<AActor*> BoxActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABoxAsset::StaticClass(), BoxActors);

	//범위기반 for 루프, 저장된 액터를 하나씩 순회
	for (AActor* BoxActor : BoxActors) 
	{
		ABoxAsset* BoxAsset = Cast<ABoxAsset>(BoxActor);
		// 메쉬 꺼내기
		UStaticMeshComponent* BoxComp = BoxAsset->Box;
		
		// 새로운 위치 계산
		FVector OrbitPosition = BlackHoleCenter + FVector(
			FMath::Cos(FMath::DegreesToRadians(CurrentAngle)),
			FMath::Sin(FMath::DegreesToRadians(CurrentAngle)),
			0) * R;

		// 박스를 해당 위치로 이동
		FVector Direction = OrbitPosition - BoxComp->GetComponentLocation();
		BoxComp->SetPhysicsLinearVelocity(Direction.GetSafeNormal() * OrbitSpeed,false,"None");
	}

	// 똑같이, 체인도 조사해서 포함시키기
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

void ABlackHole::DeactiveBlackhole()
{
	Player->GetMesh()->SetSimulatePhysics(false);

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
}
