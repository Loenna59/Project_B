// Fill out your copyright notice in the Description page of Project Settings.


#include "LuggageHole.h"

#include "Project_B/Utilities/LogMacro.h"
#include "Components/BoxComponent.h"


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
	LOG_SCREEN("오버랩 시작");
}

void ALuggageHole::OnEndOverlapBind(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	LOG_SCREEN("오버랩 엔드");
}

