// Fill out your copyright notice in the Description page of Project Settings.


#include "PodiumGameMode.h"
#include "PodiumGameState.h"
#include "GameFramework/PlayerState.h"
#include "Project_B/Maps/LobbyMap/BanimalsGameInstance.h"
#include "Project_B/Utilities/LogMacro.h"

void APodiumGameMode::BeginPlay()
{
	Super::BeginPlay();
	//TODO: WinnerKey gi로부터 가져오기
	UBanimalsGameInstance* gi = Cast<UBanimalsGameInstance>(GetWorld()->GetGameInstance());
	WinnerKeys = gi->WinnerKeys;
}

void APodiumGameMode::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);
	APodiumGameState* gs = Cast<APodiumGameState>(GetWorld()->GetGameState());
	if (gs)
	{
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, [NewPlayer, this]()
		{
			if (NewPlayer && NewPlayer->GetPawn())
			{
				APodiumGameState* gs = Cast<APodiumGameState>(GetWorld()->GetGameState());
				if (gs)
				{
					const FUniqueNetIdRepl& NetIdRepl = NewPlayer->GetPlayerState<APlayerState>()->GetUniqueId();
					FString key;
					
					if (NetIdRepl.IsValid())
					{
						TSharedPtr<const FUniqueNetId> NetId = NetIdRepl.GetUniqueNetId();
						key = NetId->ToString();
					}
					
					if (WinnerKeys.Find(key))
					{
						UE_LOG(LogTemp, Error, TEXT("이겼다!!!!!!!"));
						gs->InitPlayerLoc(NewPlayer->GetPawn(), true);
					}
					else
					{
						UE_LOG(LogTemp, Error, TEXT("졌어ㅠㅠㅠㅠㅠㅠㅠㅠㅠㅠㅠㅠㅠㅠ"));
						gs->InitPlayerLoc(NewPlayer->GetPawn(), false);
					}
					
					gs->InitPodiumCamera(NewPlayer->GetPlayerState<APlayerState>()->GetPlayerController());
				}
			}
		}, BeginDelay, false); 
	}
	else
	{
		LOG_ERROR(this,TEXT("올바른 게임스테이트 미할당"));
	}
}
