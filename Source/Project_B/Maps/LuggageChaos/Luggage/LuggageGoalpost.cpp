// Fill out your copyright notice in the Description page of Project Settings.


#include "LuggageGoalpost.h"

#include "Luggage.h"
#include "LuggageManager.h"
#include "Project_B/Utilities/LogMacro.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Project_B/Maps/LuggageChaos/Game/LuggageChaosGameMode.h"
#include "Project_B/Maps/LuggageChaos/Game/LuggageChaosGameState.h"


ALuggageGoalpost::ALuggageGoalpost()
{
	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	SetRootComponent(Box);
	Box->SetBoxExtent(FVector(220.000000,175.000000,300.000000));
}

void ALuggageGoalpost::BeginPlay()
{
	Super::BeginPlay();
	Box->OnComponentBeginOverlap.AddDynamic(this,&ALuggageGoalpost::OnBeginOverlapBind);
	Box->OnComponentEndOverlap.AddDynamic(this,&ALuggageGoalpost::OnEndOverlapBind);

	LuggageManager = Cast<ALuggageManager>(UGameplayStatics::GetActorOfClass(GetWorld(),ALuggageManager::StaticClass()));
	
	if (LuggageManager == nullptr)
	{
		LOG_ERROR(this, "러기지 매니저 존재하지 않음");
	}
}

void ALuggageGoalpost::OnBeginOverlapBind(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit)
{
	ALuggage* luggage = Cast<ALuggage>(OtherActor);
	if (luggage != nullptr)
	{
		Server_AddScore(luggage->Point);
	}
}

void ALuggageGoalpost::OnEndOverlapBind(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->IsA(ACharacter::StaticClass()))
	{
		return;
	}

	ALuggage* lug = Cast<ALuggage>(OtherActor);
	if (lug != nullptr)
	{
		
		LuggageManager->OnReturnPooledObject.Broadcast(lug, Team);
	}
}

void ALuggageGoalpost::Server_AddScore_Implementation(const uint8 point)
{
	Net_AddScore(point);
}

void ALuggageGoalpost::Net_AddScore_Implementation(const uint8 point)
{
	ALuggageChaosGameState* gs = Cast<ALuggageChaosGameState>(UGameplayStatics::GetGameState(GetWorld()));
	if (gs == nullptr)
	{
		LOG_ERROR(this,"게임스테이트 설정 필요");
		return;
	}
	gs->AddScore(Team, point);
}
