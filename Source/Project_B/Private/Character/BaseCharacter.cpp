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
#include "Components/AudioComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
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

	// VoiceComp = CreateDefaultSubobject<UAudioComponent>(TEXT("VoiceComp"));
	// VoiceComp->SetupAttachment(RootComponent);
	// VoiceComp->bAutoActivate = false;

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

	OneHandedSocket = CreateDefaultSubobject<USceneComponent>(TEXT("OneHandedSocket"));
	OneHandedSocket->SetupAttachment(GetMesh(), TEXT("OneHanded"));

	Sunglasses = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Sunglasses"));
	Sunglasses->SetupAttachment(GetMesh(), TEXT("Sunglasses"));

	ConstructorHelpers::FObjectFinder<UStaticMesh> SunglassesMesh(TEXT("/Game/Assets/_Objects/Sunglasses/Sunglasses.Sunglasses"));

	if (SunglassesMesh.Succeeded())
	{
		Sunglasses->SetStaticMesh(SunglassesMesh.Object);
	}

	Sunglasses->SetVisibility(false);
	
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

	ConstructorHelpers::FObjectFinder<USoundBase> temp_hitSound(TEXT("/Game/Assets/Sounds/Hit.Hit"));

	if (temp_hitSound.Succeeded())
	{
		HitSound = temp_hitSound.Object;
		// VoiceComp->SetSound(temp_hitSound.Object);
	}
	
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	SetReplicateMovement(true);
	SetSunglasses(false);

	PhysicalAnimationComp->SetSkeletalMeshComponent(GetMesh());
	
	APlayerController* pc = Cast<APlayerController>(Controller);

	if (pc)
	{
		InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
			pc->GetLocalPlayer());

		if (InputSubsystem)
		{
			InputSubsystem->AddMappingContext(IMC, 0);
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
	DOREPLIFETIME(ABaseCharacter, bIsKnockdown);
}

void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* InputComp = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	if (InputComp)
	{
		MoveComp->SetupInputBiding(InputComp);
		AttackComp->SetupInputBiding(InputComp);
		PickComp->SetupInputBiding(InputComp);

		InputComp->BindAction(InputActionUnequip, ETriggerEvent::Started, this, &ABaseCharacter::Unequip);
	}
}

void ABaseCharacter::OnHit(EAttackType Type, FVector NormalPoint, float damage)
{
	if (bIsKnockdown)
	{
		return;
	}
	
	switch (Type)
	{
	case EAttackType::HAMMER:
	case EAttackType::HEAD_BUTT:
	case EAttackType::KICK:
		Client_SetEnableInput(false);
		break;
	default:
		break;
	}
	Server_OnPlayHitMontage(Type, NormalPoint);
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

	Weapon->CancelDisappear();
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

	USceneComponent* Socket = OwnedWeapon->GetWeaponType() == EWeaponType::OneHanded? OneHandedSocket : TwoHandedSocket;
	OwnedWeapon->AttachToComponent(Socket, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	
	// 애니메이션 변경
	if (AnimInstance)
	{
		AnimInstance->CurrentWeaponType = OwnedWeapon->GetWeaponType();
	}

	// 팔의 physics를 꺼줘야함
	LeftArmPhysicsAnimComp->TogglePhysicalAnimation(OwnedWeapon->GetWeaponType() == EWeaponType::OneHanded);
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
	if (CheckAndStopKnockdown())
	{
		return;
	}
	
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
	if (!Weapon)
	{
		return;
	}
	
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

void ABaseCharacter::Server_OnPlayHitMontage_Implementation(EAttackType Type, FVector NormalPoint)
{
	// if (!IsLocallyControlled())
	// {
	// 	return;
	// }
	//
	LOG_SCREEN("Hit");
	
	float Power = 1000.f;
	FVector LaunchVelocity = NormalPoint * Power + FVector(0, 0, 100.f);

	FVector WorldHitDir = LaunchVelocity.GetSafeNormal();
	FVector LocalHitDir = GetActorTransform().InverseTransformVectorNoScale(WorldHitDir);

	float ForwardDot = FVector::DotProduct(LocalHitDir, FVector::ForwardVector);
	float SideDot = FVector::DotProduct(LocalHitDir, FVector::RightVector);

	bool bIsKnockdownHit = false;

	switch (Type)
	{
	case EAttackType::PUNCH:
	case EAttackType::BOTTLE:
	case EAttackType::CROSS_BOW:
	{
		++CurrentNormalHitCount;

		GetWorldTimerManager().ClearTimer(ResetHitCountTimerHandle);

		int32 MaxCount = Type == EAttackType::PUNCH? KnockdownPunchCount : KnockdownHitCount;
		
		if (CurrentNormalHitCount >= MaxCount)
		{
			bIsKnockdownHit = true;
			break;
		}
		
		TWeakObjectPtr<ABaseCharacter> WeakThis = this;
		
		GetWorldTimerManager().SetTimer(
			ResetHitCountTimerHandle,
			[WeakThis]()
			{
				if (WeakThis.IsValid())
				{
					WeakThis->CurrentNormalHitCount = 0;
				}
			},
			HitCountResetTime,
			false
		);
		break;
	}
	default:
		bIsKnockdownHit = true;
		break;
	}
	
	Multicast_OnPlayHitMontage(bIsKnockdownHit, ForwardDot, SideDot, LaunchVelocity);
}

void ABaseCharacter::Multicast_OnPlayHitMontage_Implementation(bool bIsKnockdownHit, float ForwardDot, float SideDot, FVector LaunchVelocity)
{
	if (bIsKnockdownHit)
	{
		// LOG_SCREEN("%s", *LaunchVelocity.GetSafeNormal().ToString());
		bIsKnockdown = true;
		CurrentNormalHitCount = 0;
		Unequip();
		LaunchCharacter(LaunchVelocity, true, true);

		UGameplayStatics::SpawnSoundAttached(HitSound, RootComponent);
		
		// VoiceComp->Play();

		if (ForwardDot > 0.7f)
		{
			PlayAnimMontage(KnockdownMontage, 1.f, TEXT("Forward"));
			return;
		}
	
		if (ForwardDot < -0.7f)
		{
			PlayAnimMontage(KnockdownMontage, 1.f, TEXT("Backward"));
			return;
		}
	
		if (SideDot > 0)
		{
			PlayAnimMontage(KnockdownMontage, 1.f, TEXT("Right"));
			return;
		}
	
		PlayAnimMontage(KnockdownMontage, 1.f, TEXT("Left"));
		return;
	}
	
	if (AnimInstance)
	{
		AnimInstance->StartHitProcess(ForwardDot, SideDot);
	}
}

void ABaseCharacter::Client_SetEnableInput_Implementation(bool bEnable)
{
	if (InputSubsystem)
	{
		if (bEnable)
		{
			InputSubsystem->AddMappingContext(IMC, 0);
			return;
		}
		InputSubsystem->RemoveMappingContext(IMC);

		TWeakObjectPtr<ABaseCharacter> WeakThis = this;

		GetWorldTimerManager().SetTimer
		(
			KnockdownTimerHandle,
			[WeakThis]()
			{
				if (WeakThis.IsValid())
				{
					WeakThis->Client_SetEnableInput(true);
				}
			},
			KnockdownTime,
			false
		);
	}	
}

bool ABaseCharacter::CheckAndStopKnockdown()
{
	if (AnimInstance->Montage_IsPlaying(KnockdownMontage))
	{
		Server_CheckAndStopKnockdown();

		return true;
	}

	return false;
}

void ABaseCharacter::SetSunglasses(bool bEquip)
{
	Sunglasses->SetVisibility(bEquip);
}

void ABaseCharacter::Server_CheckAndStopKnockdown_Implementation()
{
	Multicast_CheckAndStopKnockdown();
}

void ABaseCharacter::Multicast_CheckAndStopKnockdown_Implementation()
{
	PlayAnimMontage(GetupAnimMontage, 1.f, "1");
	bIsKnockdown = false;
}
