// Fill out your copyright notice in the Description page of Project Settings.


#include "PodiumGameMode.h"
#include "PodiumGameState.h"
#include "GameFramework/PlayerState.h"
#include "Project_B/Maps/LobbyMap/BanimalsGameInstance.h"
#include "Project_B/Utilities/LogMacro.h"

void APodiumGameMode::BeginPlay()
{
	Super::BeginPlay();

	UBanimalsGameInstance* gi = Cast<UBanimalsGameInstance>(GetWorld()->GetGameInstance());
	WinnerKeys = gi->WinnerKeys;
	
	for (int i = 0; i<WinnerKeys.Num(); i++)
	{
		UE_LOG(LogTemp,Error,TEXT("gi에 저장된 승리자: %s"), *WinnerKeys[i]);
	}
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
				const FUniqueNetIdRepl& NetIdRepl = NewPlayer->GetPlayerState<APlayerState>()->GetUniqueId();
				FString key;
				
				if (NetIdRepl.IsValid())
				{
					TSharedPtr<const FUniqueNetId> NetId = NetIdRepl.GetUniqueNetId();
					key = NetId->ToString();
					LOG_PRINT(TEXT("접속한 플레이어 키: %s"), *key);
				}

				if (WinnerKeys.IsEmpty())
				{
					LOG_PRINT(TEXT("승리자 없음"));
				}
				
				for (int i = 0; i<WinnerKeys.Num(); i++)
				{
					UE_LOG(LogTemp,Error,TEXT("gi에 저장된 승리자: %s"), *WinnerKeys[i]);
				}

				APodiumGameState* gs = Cast<APodiumGameState>(GetWorld()->GetGameState());
				if (WinnerKeys.Find(key))
				{
					UE_LOG(LogTemp, Error, TEXT("%s번 이겼다!!!!!!!"), *key);
					gs->InitPlayer(Cast<APlayerController>(NewPlayer), true);
				}
				else
				{
					UE_LOG(LogTemp, Error, TEXT("%s번 졌어ㅠㅠㅠㅠㅠㅠㅠㅠㅠㅠㅠㅠ"),*key);
					gs->InitPlayer(Cast<APlayerController>(NewPlayer), false);
				}
			}
		}, BeginDelay, false); 
	}
	else
	{
		LOG_ERROR(this,TEXT("올바른 게임스테이트 미할당"));
	}
}
