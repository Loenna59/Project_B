// Fill out your copyright notice in the Description page of Project Settings.


#include "Project_B/Maps/BlackHole/Public/BlackholePlayerState.h"

#include "Project_B/Maps/LobbyMap/LobbyGameState.h"
#include "Project_B/Maps/LobbyMap/Widget/BlackholeLobbyUI.h"

/*void ABlackholePlayerState::OnRep_UniqueId()
{
	Super::OnRep_UniqueId();

	UE_LOG(LogTemp, Display, TEXT("OnRep_UniqueId"));
	ALobbyGameState* gs = Cast<ALobbyGameState>(GetWorld()->GetGameState());
	gs->BlackholeLobbyWidget->UpdateImage();
}*/
void ABlackholePlayerState::GetLifetimeReplicatedProps(
	TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}
