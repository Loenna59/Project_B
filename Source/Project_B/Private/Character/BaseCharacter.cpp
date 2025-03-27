#include "Character/BaseCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "Camera/CameraComponent.h"
#include "Character/BaseCharacterAnimInstance.h"
#include "Character/BaseCharacterArmComponent.h"
#include "Character/BaseCharacterAttackComponent.h"
#include "Character/BaseCharacterMoveComponent.h"
#include "Character/BaseCharacterPhysicsAnimComponent.h"
#include "Character/BaseCharacterPickComponent.h"
#include "Character/HeadPhysicsAnimComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "PhysicsEngine/PhysicalAnimationComponent.h"
#include "Project_B/Maps/BlackHole/Public/GravityComponent.h"
#include "Project_B/Utilities/LogMacro.h"

ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	GetCapsuleComponent()->SetCapsuleRadius(50.f);
	GetCapsuleComponent()->SetCapsuleHalfHeight(100.f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;

	GetMesh()->SetRelativeLocation(FVector(0, 0, -102.f));
	GetMesh()->SetRelativeRotation(FRotator(0, -90.f, 0));

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->TargetArmLength = 600.f;
	SpringArmComp->SocketOffset = FVector(0, 0, 205.f);
	SpringArmComp->bUsePawnControlRotation = true;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(SpringArmComp);
	CameraComp->SetRelativeRotation(FRotator(-25.f, 0, 0));
	CameraComp->SetFieldOfView(90.f);
	CameraComp->bUsePawnControlRotation = false;

	MoveComp = CreateDefaultSubobject<UBaseCharacterMoveComponent>(TEXT("MoveComp"));
	MoveComp->RegisterComponent();
	MoveComp->SetIsReplicated(true);
	
	AttackComp = CreateDefaultSubobject<UBaseCharacterAttackComponent>(TEXT("AttackComp"));
	AttackComp->RegisterComponent();
	AttackComp->SetIsReplicated(true);

	PickComp = CreateDefaultSubobject<UBaseCharacterPickComponent>(TEXT("PickComp"));
	PickComp->RegisterComponent();
	PickComp->SetIsReplicated(true);
	
	PhysicalAnimationComp = CreateDefaultSubobject<UPhysicalAnimationComponent>(TEXT("PhysicalAnimComp"));
	PhysicalAnimationComp->RegisterComponent();
	
	HeadPhysicsAnimComp = CreateDefaultSubobject<UHeadPhysicsAnimComponent>(TEXT("HeadPhysicsAnimComp"));
	HeadPhysicsAnimComp->RegisterComponent();
	HeadPhysicsAnimComp->SetIsReplicated(true);
	
	LeftArmPhysicsAnimComp = CreateDefaultSubobject<UBaseCharacterArmComponent>(TEXT("LeftArmPhysicsAnimComp"));
	LeftArmPhysicsAnimComp->RegisterComponent();
	LeftArmPhysicsAnimComp->SetIsReplicated(true);
	
	RightArmPhysicsAnimComp = CreateDefaultSubobject<UBaseCharacterArmComponent>(TEXT("RightArmPhysicsAnimComp"));
	RightArmPhysicsAnimComp->RegisterComponent();
	RightArmPhysicsAnimComp->SetIsReplicated(true);

	RightFootPhysicsAnimComp = CreateDefaultSubobject<UBaseCharacterPhysicsAnimComponent>(TEXT("RightFootPhysicsAnimComp"));
	RightFootPhysicsAnimComp->RegisterComponent();
	RightFootPhysicsAnimComp->SetIsReplicated(true);
	
	GravityComp = CreateDefaultSubobject<UGravityComponent>(TEXT("GravityComp"));
	GravityComp->RegisterComponent();
	// GravityComp->SetNetAddressable();
	GravityComp->SetIsReplicated(true);

	ConstructorHelpers::FObjectFinder<UInputMappingContext> tmp_imc(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Input/IMC_Default.IMC_Default'"));

	if (tmp_imc.Succeeded())
	{
		IMC = tmp_imc.Object;
	}

}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	SetReplicateMovement(true);

	PhysicalAnimationComp->SetSkeletalMeshComponent(GetMesh());
	
	APlayerController* pc = Cast<APlayerController>(Controller);

	if (pc)
	{
		auto subSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(pc->GetLocalPlayer());

		if (subSystem)
		{
			subSystem->AddMappingContext(IMC, 0);
		}
	}

	GetMesh()->SetAngularDamping(2.0f);

	CurrentHealth = MaxHealth;

	AnimInstance = Cast<UBaseCharacterAnimInstance>(GetMesh()->GetAnimInstance());
}

void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* pi = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	if (pi)
	{
		MoveComp->SetupInputBiding(pi);
		AttackComp->SetupInputBiding(pi);
		PickComp->SetupInputBiding(pi);
	}

}

void ABaseCharacter::OnHit(EAttackType Type, FVector NormalPoint, float damage)
{
	float ForwardDot = FVector::DotProduct(GetActorForwardVector(), NormalPoint);

	float clampedForwardDot = FMath::Clamp(ForwardDot, -1.f, 1.f);

	float SideDot = FVector::DotProduct(GetActorRightVector(), NormalPoint);

	if (HasAuthority())
	{
		Multicast_OnPlayHitMontage(Type, clampedForwardDot, SideDot);
	}
	else
	{
		Server_OnPlayHitMontage(Type, clampedForwardDot, SideDot);
	}
}

void ABaseCharacter::Server_OnPlayHitMontage_Implementation(EAttackType Type, float ForwardDot, float SideDot)
{
	Multicast_OnPlayHitMontage(Type, ForwardDot, SideDot);
}

void ABaseCharacter::Multicast_OnPlayHitMontage_Implementation(EAttackType Type, float ForwardDot, float SideDot)
{
	switch (Type)
	{
	case EAttackType::PUNCH:
	case EAttackType::BOTTLE:
	case EAttackType::CROSS_BOW:
		if (AnimInstance)
		{
			AnimInstance->StartHitProcess(ForwardDot, SideDot);
		}
		break;
	default:
		if (ForwardDot > 0.6f)
		{
			PlayAnimMontage(KnockdownMontage, 1.f, TEXT("Forward"));
			return;
		}
	
		if (ForwardDot < -0.6f)
		{
			PlayAnimMontage(KnockdownMontage, 1.f, TEXT("Backward"));
			return;
		}
	
		if (SideDot > 0)
		{
			PlayAnimMontage(KnockdownMontage, 1.f, TEXT("Left"));
			return;
		}
	
		PlayAnimMontage(KnockdownMontage, 1.f, TEXT("Right"));
		break;
	}
}

