// Fill out your copyright notice in the Description page of Project Settings.


#include "LuggagePlayerState.h"

#include "LuggageChaosGameState.h"
#include "Project_B/Utilities/LogMacro.h"

void ALuggagePlayerState::Server_Win_Implementation()
{
	ALuggageChaosGameState* gs = Cast<ALuggageChaosGameState>(GetWorld()->GetGameState());

	const FUniqueNetIdRepl& NetIdRepl = GetUniqueId();
	FString key;
				
	if (NetIdRepl.IsValid())
	{
		TSharedPtr<const FUniqueNetId> NetId = NetIdRepl.GetUniqueNetId();
		key = NetId->ToString();
	}
	gs->AddWinner(key);
}
