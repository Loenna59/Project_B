// Fill out your copyright notice in the Description page of Project Settings.


#include "LuggageHole.h"

#include "Luggage.h"
#include "Project_B/Utilities/LogMacro.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Project_B/Maps/LuggageChaos/Game/LuggageChaosGameMode.h"


ALuggageHole::ALuggageHole()
{
	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	SetRootComponent(Box);
	Box->SetBoxExtent(FVector(220.000000,175.000000,300.000000));
}

void ALuggageHole::BeginPlay()
{
	Super::BeginPlay();
	Box->OnComponentBeginOverlap.AddDynamic(this,&ALuggageHole::OnBeginOverlapBind);
	Box->OnComponentEndOverlap.AddDynamic(this,&ALuggageHole::OnEndOverlapBind);
}

void ALuggageHole::OnBeginOverlapBind(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit)
{
	//테스트
	if (Team)
	{
		LOG_SCREEN("블루팀!");
	}
	else
	{
		LOG_SCREEN("레드팀!");
	}
	
	ALuggage* luggage = Cast<ALuggage>(OtherActor);
	if (luggage != nullptr)
	{
		ALuggageChaosGameMode* gm = Cast<ALuggageChaosGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
		if (gm == nullptr)
		{
			LOG_ERROR(this,"게임모드 없음");
			return;
		}

		if (Team)
		{
			gm->AddScoreBlue(luggage->Point);
		}
		else
		{
			gm->AddScoreRed(luggage->Point);
		}
	}
}

void ALuggageHole::OnEndOverlapBind(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->IsA(ACharacter::StaticClass()))
	{
		return;
	}
	
	OtherActor->Destroy();
}

