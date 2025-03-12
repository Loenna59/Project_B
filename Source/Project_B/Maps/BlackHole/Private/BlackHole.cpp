// Fill out your copyright notice in the Description page of Project Settings.


#include "Project_B/Maps/BlackHole/Public/BlackHole.h"

#include "Character/BaseCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Project_B/Maps/BlackHole/Public/BoxAsset.h"
#include "Project_B/Maps/BlackHole/Public/Chain.h"


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
		ActivateBlackhole();
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

void ABlackHole::FirstRotation()
{
	// 첫 라운드에는, 최대 난간까지만 끌어올리고 블랙홀 주위를 회전하지도 않음
	// 물체를 잡고 있다면 가장 바깥에서 돌고 있도록 하자
	// 잡고 있지않다면 난간정도 위치까지 떠오르게 하자
}

void ABlackHole::SecondRotation()
{
	// 두번째에서는 물건을 들고 있다면 생존하고, 무조건 회전한다
	// 1페이즈와 동일하지만 회전을 추가해주자
	// 물건을 들고 있지 않다면 난간 안으로 들어가서 죽음
}

void ABlackHole::ThirdRotation()
{
	// 무거운 물체만 생존한다
}

void ABlackHole::FourthRotation()
{
	// 사슬만 생존한다
}

void ABlackHole::DeactivateBlackhole()
{
	// 플레이어의 캡슐 물리 일단 꺼주기
	ABaseCharacter* Player = Cast<ABaseCharacter>(UGameplayStatics::GetActorOfClass(GetWorld(), ABaseCharacter::StaticClass()));
	UCapsuleComponent* capsule = Player->GetCapsuleComponent();
	capsule->SetSimulatePhysics(false);
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
