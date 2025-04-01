// Fill out your copyright notice in the Description page of Project Settings.


#include "Project_B/Maps/BlackHole/Public/BlackholeSpectator.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "Blueprint/UserWidget.h"
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
		UE_LOG(LogTemp,Display,TEXT("IMC_Spectator 바인딩추가"));
	}
	
	pc = Cast<APlayerController>(GetController());
	
	PlayerInputComponent->ClearActionBindings();
	PlayerInputComponent->ClearAxisBindings();
	PlayerInputComponent->AxisBindings.Empty();
	UE_LOG(LogTemp,Display,TEXT("플레이어 바인딩 초기화"));

	UE_LOG(LogTemp, Warning, TEXT("Possessed: %p , LocalPlayerController: %p"), (void*)Controller, GetWorld()->GetFirstPlayerController());

	auto pi = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	pi->BindAction(IA_Fire, ETriggerEvent::Started, this, &ABlackholeSpectator::SpawnProjectile);
}

void ABlackholeSpectator::SpawnProjectile()
{
	UE_LOG(LogTemp, Warning, TEXT("Spawning Projectile"));
	
	// Projectile 발사 위치
	FVector SpawnLocation = GetActorLocation() + GetActorForwardVector() * 100.0f;
	FActorSpawnParameters SpawnParams;

	// Projectile 생성
	// GetWorld()->SpawnActor<AProjectileClass>(ProjectileClass, SpawnLocation, Rotation, SpawnParams);
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

