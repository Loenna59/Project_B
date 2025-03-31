#include "Character/BaseCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
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
#include "Net/UnrealNetwork.h"
#include "PhysicsEngine/PhysicalAnimationComponent.h"
#include "Project_B/Maps/BlackHole/Public/GravityComponent.h"
#include "Project_B/Utilities/LogMacro.h"
#include "Weapon/Weapon.h"

ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	bAlwaysRelevant = true;

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
	MoveComp->SetIsReplicated(true);
	MoveComp->SetNetAddressable();
	
	AttackComp = CreateDefaultSubobject<UBaseCharacterAttackComponent>(TEXT("AttackComp"));
	AttackComp->SetIsReplicated(true);
	AttackComp->SetNetAddressable();

	PickComp = CreateDefaultSubobject<UBaseCharacterPickComponent>(TEXT("PickComp"));
	PickComp->SetIsReplicated(true);
	PickComp->SetNetAddressable();
	
	PhysicalAnimationComp = CreateDefaultSubobject<UPhysicalAnimationComponent>(TEXT("PhysicalAnimComp"));
	PhysicalAnimationComp->SetIsReplicated(true);
	PhysicalAnimationComp->SetNetAddressable();
	
	HeadPhysicsAnimComp = CreateDefaultSubobject<UHeadPhysicsAnimComponent>(TEXT("HeadPhysicsAnimComp"));
	HeadPhysicsAnimComp->SetIsReplicated(true);
	HeadPhysicsAnimComp->SetNetAddressable();
	
	LeftArmPhysicsAnimComp = CreateDefaultSubobject<UBaseCharacterArmComponent>(TEXT("LeftArmPhysicsAnimComp"));
	LeftArmPhysicsAnimComp->SetIsReplicated(true);
	LeftArmPhysicsAnimComp->SetNetAddressable();
	
	RightArmPhysicsAnimComp = CreateDefaultSubobject<UBaseCharacterArmComponent>(TEXT("RightArmPhysicsAnimComp"));
	RightArmPhysicsAnimComp->SetIsReplicated(true);
	RightArmPhysicsAnimComp->SetNetAddressable();

	RightFootPhysicsAnimComp = CreateDefaultSubobject<UBaseCharacterPhysicsAnimComponent>(TEXT("RightFootPhysicsAnimComp"));
	RightFootPhysicsAnimComp->SetIsReplicated(true);
	RightFootPhysicsAnimComp->SetNetAddressable();
	
	GravityComp = CreateDefaultSubobject<UGravityComponent>(TEXT("GravityComp"));
	GravityComp->SetIsReplicated(true);
	GravityComp->SetNetAddressable();

	TwoHandedSocket = CreateDefaultSubobject<USceneComponent>(TEXT("TwoHandedSocket"));
	TwoHandedSocket->SetupAttachment(GetMesh(), TEXT("TwoHanded"));

	ConstructorHelpers::FObjectFinder<UInputMappingContext> tmp_imc(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Input/IMC_Default.IMC_Default'"));

	if (tmp_imc.Succeeded())
	{
		IMC = tmp_imc.Object;
	}

	ConstructorHelpers::FObjectFinder<UInputAction> temp_ia(TEXT("/Game/Input/Actions/IA_Unequip.IA_Unequip"));

	if (temp_ia.Succeeded())
	{
		InputActionUnequip = temp_ia.Object;
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

	OnCalculateSpeedByMass.BindUObject(this, &ABaseCharacter::CalculateSpeedByMass);
}

void ABaseCharacter::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABaseCharacter, OwnedWeapon);
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

		pi->BindAction(InputActionUnequip, ETriggerEvent::Started, this, &ABaseCharacter::Unequip);
	}
}

void ABaseCharacter::OnHit(EAttackType Type, FVector NormalPoint, float damage)
{
	float ForwardDot = FVector::DotProduct(GetActorForwardVector(), NormalPoint);

	float clampedForwardDot = FMath::Clamp(ForwardDot, -1.f, 1.f);

	float SideDot = FVector::DotProduct(GetActorRightVector(), NormalPoint);

	Server_OnPlayHitMontage(Type, clampedForwardDot, SideDot);
}

void ABaseCharacter::TakeWeapon(class AWeapon* Weapon)
{
	Server_TakeWeapon(Weapon);
}

void ABaseCharacter::Server_TakeWeapon_Implementation(class AWeapon* Weapon)
{
	if (bHasWeapon)
	{
		return;
	}

	OwnedWeapon = Weapon;
	OwnedWeapon->SetOwner(this);
	
	AttachWeapon();
}

void ABaseCharacter::AttachWeapon()
{
	if (OwnedWeapon == nullptr)
	{
		return;
	}
	
	bHasWeapon = true;

	OwnedWeapon->ToggleSimulatePhysics(false);
	OwnedWeapon->AttachToComponent(TwoHandedSocket, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	
	// 애니메이션 변경
	if (AnimInstance)
	{
		AnimInstance->CurrentWeaponType = OwnedWeapon->GetWeaponType();
	}

	// 팔의 physics를 꺼줘야함
	LeftArmPhysicsAnimComp->TogglePhysicalAnimation(false);
	RightArmPhysicsAnimComp->TogglePhysicalAnimation(false);
}

void ABaseCharacter::OnWeaponAttackTraceChannel()
{
	if (!bHasWeapon)
	{
		return;
	}

	if (OwnedWeapon == nullptr)
	{
		return;
	}

	OwnedWeapon->OnAttackTraceChannel();
}

void ABaseCharacter::Unequip()
{
	Server_UnequipWeapon();
}

void ABaseCharacter::CalculateSpeedByMass(float Mass)
{
	if (MoveComp)
	{
		MoveComp->CalculateSpeedByMass(Mass);
	}
}

void ABaseCharacter::Server_UnequipWeapon_Implementation()
{
	if (!bHasWeapon)
	{
		return;
	}

	AWeapon* Weapon = OwnedWeapon;
	OwnedWeapon->SetOwner(nullptr);
	OwnedWeapon = nullptr;

	Multicast_UnequipWeapon(Weapon);
}

void ABaseCharacter::Multicast_UnequipWeapon_Implementation(AWeapon* Weapon)
{
	Weapon->ToggleSimulatePhysics(true);
	Weapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	
	bHasWeapon = false;

	// 애니메이션 변경
	if (AnimInstance)
	{
		AnimInstance->CurrentWeaponType = EWeaponType::None;
	}
	
	LeftArmPhysicsAnimComp->TogglePhysicalAnimation(true);
	RightArmPhysicsAnimComp->TogglePhysicalAnimation(true);
}

void ABaseCharacter::Server_OnPlayHitMontage_Implementation(EAttackType Type, float ForwardDot, float SideDot)
{
	// if (!IsLocallyControlled())
	// {
	// 	return;
	// }
	//
	LOG_SCREEN("Hit");
	
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

