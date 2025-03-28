// Fill out your copyright notice in the Description page of Project Settings.


#include "Project_B/Maps/BlackHole/Public/DestroyZone.h"

#include "DataTableEditorUtils.h"
#include "Components/SphereComponent.h"
#include "GameFramework/PlayerState.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Project_B/Maps/Base/BanimalsGameState.h"
#include "Project_B/Maps/BlackHole/Public/BlackHole.h"
#include "Project_B/Maps/BlackHole/Public/BlackholeGameMode.h"
#include "Project_B/Maps/BlackHole/Public/BlackholeGameState.h"
#include "Project_B/Maps/LobbyMap/BanimalsGameInstance.h"
#include "Project_B/Utilities/LogMacro.h"


// Sets default values
ADestroyZone::ADestroyZone()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 외관
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);
	BottomBlade = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BottomBlade"));
	BottomBlade->SetupAttachment(RootComponent);
	BottomBlade->SetRelativeLocation(FVector(0, 0, 200));
	BottomBlade->SetRelativeRotation(FRotator(0,0,-90));
	
	BottomRotator = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BottomRotator"));
	BottomRotator->SetupAttachment(RootComponent);
	BottomRotator->SetRelativeRotation(FRotator(0,0,90));
	
	DestroyZoneSphere = CreateDefaultSubobject<USphereComponent>(TEXT("DestroyZoneSphere"));
	DestroyZoneSphere->SetupAttachment(RootComponent);
	DestroyZoneSphere->SetRelativeLocation(FVector(0, 0, 0));
	DestroyZoneSphere->SetRelativeScale3D(FVector(20));

	// 회전
	BladeRotateComp = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("BladeRotateComp"));
	BladeRotateComp->SetUpdatedComponent(BottomBlade);
	BladeRotateComp->RotationRate = FRotator(RotateSpeed,0,0);
	
	RotateComp = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("RotateComp"));
	RotateComp->SetUpdatedComponent(BottomRotator);
	RotateComp->RotationRate = FRotator(RotateSpeed,0,0);
	RotateComp->PivotTranslation = FVector(0, -1, 0);
	
	// 외관 넣어주기
	ConstructorHelpers::FObjectFinder<UStaticMesh>TempBlade(TEXT("/Script/Engine.StaticMesh'/Game/Assets/Blackhole/Blackhole_BottomBlade.Blackhole_BottomBlade'"));
	if (TempBlade.Succeeded())
	{
		BottomBlade->SetStaticMesh(TempBlade.Object);
	}
	ConstructorHelpers::FObjectFinder<UStaticMesh>TempRotator(TEXT("/Script/Engine.StaticMesh'/Game/Assets/Blackhole/Blackhole_BottomRotor.Blackhole_BottomRotor'"));
	if (TempRotator.Succeeded())
	{
		BottomRotator->SetStaticMesh(TempRotator.Object);
	}

}

void ADestroyZone::OnDestroyBeginOverlap(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	const FHitResult& SweepResult)
{
	FString Key;
	
	// 서버에서만 관리
	if (!HasAuthority()) return;
	
	// TODO: 플레이어 죽음
	LOG_PRINT(TEXT("플레이어 죽음!"));
	// overlap된 액터가 플레이어면
	ABaseCharacter* player = Cast<ABaseCharacter>(OtherActor);
	if (player)
	{
		// 해당 플레이어의 PlayerController 가져오기
		APlayerController* PlayerController = Cast<APlayerController>(player->GetController());
        
		if (PlayerController)
		{
			// GameState의 OnPlayerDeath 함수 호출
			ABlackholeGameState* gs = Cast<ABlackholeGameState>(GetWorld()->GetGameState());
			if (gs)
			{
				gs->OnPlayerDeath(PlayerController);
			}
		}
	}
}

void ADestroyZone::UpdatePlayerState(const FString& playerKey)
{
	// 플레이어 상태를 변경
	UBanimalsGameInstance* gi = Cast<UBanimalsGameInstance>(GetWorld()->GetGameInstance());
	
	// 플레이어 정보 맵 참조
	TMap<FString, FPlayerInfo>& PlayerInfoMap = gi->GetPlayerInfo();
	// 플레이어 정보 존재 확인
	if (PlayerInfoMap.Contains(playerKey))
	{
		// 상태 변경
		FPlayerInfo& PlayerInfo = PlayerInfoMap[playerKey];
		PlayerInfo.bIsAlive = false;
	}
}

// Called when the game starts or when spawned
void ADestroyZone::BeginPlay()
{
	Super::BeginPlay();

	DestroyZoneSphere->SetGenerateOverlapEvents(true);
	DestroyZoneSphere->OnComponentBeginOverlap.AddDynamic(this,&ADestroyZone::OnDestroyBeginOverlap);

	// 블랙홀 찾기
	Blackhole = Cast<ABlackHole>(UGameplayStatics::GetActorOfClass(GetWorld(), ABlackHole::StaticClass()));

	Rotate(false);
}

// Called every frame
void ADestroyZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ADestroyZone::Rotate(bool bIsRotate)
{
	if (bIsRotate)
	{
		RotateSpeed = 90.f;
	}
	else
	{
		RotateSpeed = 0.f;
	}
	BladeRotateComp->RotationRate = FRotator(RotateSpeed,0,0);
	RotateComp->RotationRate = FRotator(RotateSpeed,0,0);
}
