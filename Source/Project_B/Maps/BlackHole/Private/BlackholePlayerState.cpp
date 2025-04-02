// Fill out your copyright notice in the Description page of Project Settings.


#include "Project_B/Maps/BlackHole/Public/BlackholePlayerState.h"

#include "Project_B/Maps/BlackHole/Public/BlackholeGameState.h"

void ABlackholePlayerState::Server_Win_Implementation()
{
	ABlackholeGameState* gs = Cast<ABlackholeGameState>(GetWorld()->GetGameState());

	const FUniqueNetIdRepl& NetIdRepl = GetUniqueId();
	FString key;
				
	if (NetIdRepl.IsValid())
	{
		TSharedPtr<const FUniqueNetId> NetId = NetIdRepl.GetUniqueNetId();
		key = NetId->ToString();
	}
	
	gs->AddWinner(key);
}
