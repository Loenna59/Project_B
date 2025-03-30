// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyPlayerState.h"

#include "LobbyGameState.h"
#include "Widget/BlackholeLobbyUI.h"

/*void ALobbyPlayerState::OnRep_UniqueId()
{
	Super::OnRep_UniqueId();

	UE_LOG(LogTemp, Display, TEXT("OnRep_UniqueId"));
	ALobbyGameState* gs = Cast<ALobbyGameState>(GetWorld()->GetGameState());
	gs->BlackholeLobbyWidget->UpdateImage();
}*/

void ALobbyPlayerState::OnSetUniqueId()
{
	Super::OnSetUniqueId();

	/*bool flag = true;
	UE_LOG(LogTemp, Display, TEXT("OnRep_UniqueId"));
    ALobbyGameState* gs = Cast<ALobbyGameState>(GetWorld()->GetGameState());

	if (gs)
	{
		if (gs->BlackholeLobbyWidget)
		{
			if (gs->BlackholeLobbyWidget && GetWorld()->GetFirstPlayerController()
				&& GetWorld()->GetFirstPlayerController()->PlayerState
				&& )
			{
				gs->BlackholeLobbyWidget->UpdateImage();
				flag = false;
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("No player info"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("No UI"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No GS"));
	}

	if (flag)
	{
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ALobbyPlayerState::OnSetUniqueId, 0.5f, false);
	}*/
}
