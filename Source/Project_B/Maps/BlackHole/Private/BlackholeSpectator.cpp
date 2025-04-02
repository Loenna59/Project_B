// Fill out your copyright notice in the Description page of Project Settings.


#include "Project_B/Maps/BlackHole/Public/BlackholeSpectator.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Project_B/Maps/BlackHole/Public/SpectatorItem.h"
#include "Project_B/Maps/BlackHole/Public/SpectatorUI.h"


class UEnhancedInputLocalPlayerSubsystem;
// Sets default values
ABlackholeSpectator::ABlackholeSpectator()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpectatorCam = CreateDefaultSubobject<UCameraComponent>(TEXT("SpectatorCam"));
	SpectatorCam->FieldOfView = 55.0f;
	SpectatorCam->SetWorldLocationAndRotation(FVector(0,-2620,1140), FRotator(-20, 90, 0));
	
	ConstructorHelpers::FObjectFinder<UInputMappingContext> TempIMC(TEXT("/Game/Maps/Blackhole/Input/IMC_Spectator.IMC_Spectator"));
	if (TempIMC.Succeeded())
	{
		IMC_Spectator = TempIMC.Object;
	}
	ConstructorHelpers::FObjectFinder<UInputAction> TempIA(TEXT("/Game/Maps/Blackhole/Input/IA_Fire.IA_Fire"));
	if (TempIA.Succeeded())
	{
		IA_Fire = TempIA.Object;
	}
	
	UE_LOG(LogTemp, Display, TEXT("Spectator 생성자"));
	// 위젯 넣기
	ConstructorHelpers::FClassFinder<USpectatorUI> TempSpUI(TEXT("/Game/Maps/Blackhole/Spectator/UI/WBP_Spectator.WBP_Spectator_C"));
	if (TempSpUI.Succeeded())
	{
		SpectatorUIFactory = TempSpUI.Class;
		UE_LOG(LogTemp, Display, TEXT("SpectatorUIFactory"));
	}
}

// Called when the game starts or when spawned
void ABlackholeSpectator::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABlackholeSpectator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ABlackholeSpectator::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// 기존 플레이어 인풋 제거하자
	if (UEnhancedInputLocalPlayerSubsystem* inputsys = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetWorld()->GetFirstPlayerController()->GetLocalPlayer()))
	{
		inputsys->ClearAllMappings();
		inputsys->AddMappingContext(IMC_Spectator,0);
	}
	
	pc = Cast<APlayerController>(GetController());
	
	PlayerInputComponent->ClearActionBindings();
	PlayerInputComponent->ClearAxisBindings();
	PlayerInputComponent->AxisBindings.Empty();

	UE_LOG(LogTemp, Warning, TEXT("Possessed: %p , LocalPlayerController: %p"), (void*)Controller, GetWorld()->GetFirstPlayerController());

	auto pi = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	pi->BindAction(IA_Fire, ETriggerEvent::Started, this, &ABlackholeSpectator::SpawnProjectile);
}

void ABlackholeSpectator::SpawnProjectile()
{
	UE_LOG(LogTemp, Warning, TEXT("Spawning Projectile"));
	
	UE_LOG(LogTemp, Warning, TEXT("Spawning Projectile"));
	
	// 1마우스 클릭 위치를 3D 공간에서 좌표, 방향을 구하기
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	FVector ClickLocation, ClickDirection;
	PC->DeprojectMousePositionToWorld(ClickLocation, ClickDirection);

	// trace 쏴서 충돌 지점 찾기
	FVector TraceEnd = ClickLocation + ClickDirection * 10000;
	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
	ObjectQueryParams.AddObjectTypesToQuery(ECC_PhysicsBody);
	
	bool bIsHit = GetWorld()->LineTraceSingleByObjectType(HitResult, ClickLocation, TraceEnd, ObjectQueryParams, Params);
	if (bIsHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("목적지: %s"), *HitResult.GetActor()->GetActorNameOrLabel());
		DrawDebugLine(GetWorld(), ClickLocation, HitResult.ImpactPoint, FColor::Blue, false, 2.0f, 0, 0.05f);
	}

	// 스폰 위치: 클릭한 위치
	FVector SpawnLocation = ClickLocation;

	// Projectile이 날아갈 목표 위치: 라인트레이스로 맞은 위치
	FVector TargetLocation = bIsHit ? HitResult.ImpactPoint : (ClickLocation + ClickDirection * 1000);
	FVector LaunchDirection = (TargetLocation - SpawnLocation).GetSafeNormal();

	// Projectile 생성
	FTransform SpawnTransform;
	SpawnTransform.SetLocation(SpawnLocation);
	SpawnTransform.SetRotation(LaunchDirection.Rotation().Quaternion());

	ASpectatorItem* SpawnedProjectile = GetWorld()->SpawnActor<ASpectatorItem>(SepctatorItmeFactory, SpawnTransform);
	if (SpawnedProjectile)
	{
		// Projectile이 클릭한 위치에서 목표 지점을 향해 날아가도록 설정
		UProjectileMovementComponent* ProjectileMovement = SpawnedProjectile->FindComponentByClass<UProjectileMovementComponent>();
		if (ProjectileMovement)
		{
			ProjectileMovement->Velocity = LaunchDirection * ProjectileMovement->InitialSpeed;
			ProjectileMovement->Activate();

			// 중력 영향을 받을지 여부 (필요하면 0으로)
			ProjectileMovement->ProjectileGravityScale = 0.0f; 
		}

		// Projectile이 이상한 방향을 바라보지 않도록 회전 조정
		FRotator NewRotation = LaunchDirection.Rotation();
		NewRotation.Pitch = 0;  // 상하 회전 방지
		NewRotation.Roll = 0;    // 옆으로 기울어지는 것 방지
		SpawnedProjectile->SetActorRotation(NewRotation);
	}
}

void ABlackholeSpectator::CreateSpectatorUI()
{
	if (SpectatorUIFactory) 
	{
		// 위젯 생성
		SpectatorUI = CreateWidget<USpectatorUI>(GetWorld(), SpectatorUIFactory);
		UE_LOG(LogTemp, Display, TEXT("Spectator UI created"));
		if (SpectatorUI)
		{
			SpectatorUI->AddToViewport(); // UI 화면에 추가
		}
	}
}