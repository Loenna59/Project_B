// Fill out your copyright notice in the Description page of Project Settings.


#include "LuggageGoalpost.h"

#include "Luggage.h"
#include "LuggageManager.h"
#include "Project_B/Utilities/LogMacro.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Project_B/Maps/LuggageChaos/Game/LuggageChaosGameMode.h"


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
		ALuggageChaosGameMode* gm = Cast<ALuggageChaosGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
		if (gm == nullptr)
		{
			LOG_ERROR(this,"게임모드 없음");
			return;
		}
		
		gm->AddScore(Team, luggage->Point);
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

