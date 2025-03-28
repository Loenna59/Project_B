// Fill out your copyright notice in the Description page of Project Settings.


#include "LuggageChaosGameMode.h"

#include "LuggageChaosGameState.h"
#include "GameFramework/PlayerState.h"
#include "Project_B/Utilities/LogMacro.h"


void ALuggageChaosGameMode::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);
	
	ALuggageChaosGameState* gs = Cast<ALuggageChaosGameState>(GetWorld()->GetGameState());
	if (gs)
	{
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, [NewPlayer, this]()
		{
			if (NewPlayer && NewPlayer->GetPawn())
			{
				ALuggageChaosGameState* gs = Cast<ALuggageChaosGameState>(GetWorld()->GetGameState());
				if (gs)
				{
					const FUniqueNetIdRepl& NetIdRepl = NewPlayer->GetPlayerState<APlayerState>()->GetUniqueId();
					FString key;
					
					if (NetIdRepl.IsValid())
					{
						TSharedPtr<const FUniqueNetId> NetId = NetIdRepl.GetUniqueNetId();
						key = NetId->ToString();
					}
					
					gs->InitPlayerLoc(NewPlayer->GetPawn(),key);
				}
			}
		}, 0.5f, false); 
	}
	else
	{
		LOG_ERROR(this,TEXT("올바른 게임스테이트 미할당"));
	}
}
