#include "Character/BaseCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "Camera/CameraComponent.h"
#include "Character/BaseCharacterAttackComponent.h"
#include "Character/BaseCharacterMoveComponent.h"
#include "Character/BaseCharacterPhysicsAnimComponent.h"
#include "Character/HeadPhysicsAnimComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->SetCapsuleRadius(60.f);
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
	CameraComp->SetFieldOfView(70.f);
	CameraComp->bUsePawnControlRotation = false;

	MoveComp = CreateDefaultSubobject<UBaseCharacterMoveComponent>(TEXT("MoveComp"));
	AttackComp = CreateDefaultSubobject<UBaseCharacterAttackComponent>(TEXT("AttackComp"));

	HeadPhysicsAnimComp = CreateDefaultSubobject<UHeadPhysicsAnimComponent>(TEXT("HeadPhysicsAnimComp"));
	HeadPhysicsAnimComp->SetupAttachment(RootComponent);

	LeftArmPhysicsAnimComp = CreateDefaultSubobject<UBaseCharacterPhysicsAnimComponent>(TEXT("LeftArmPhysicsAnimComp"));
	LeftArmPhysicsAnimComp->SetupAttachment(RootComponent);

	RightArmPhysicsAnimComp = CreateDefaultSubobject<UBaseCharacterPhysicsAnimComponent>(TEXT("RightArmPhysicsAnimComp"));
	RightArmPhysicsAnimComp->SetupAttachment(RootComponent);

	RightFootPhysicsAnimComp = CreateDefaultSubobject<UBaseCharacterPhysicsAnimComponent>(TEXT("RightFootPhysicsAnimComp"));
	RightFootPhysicsAnimComp->SetupAttachment(RootComponent);

	ConstructorHelpers::FObjectFinder<UInputMappingContext> tmp_imc(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Input/IMC_Default.IMC_Default'"));

	if (tmp_imc.Succeeded())
	{
		IMC = tmp_imc.Object;
	}

}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

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
	}

}

