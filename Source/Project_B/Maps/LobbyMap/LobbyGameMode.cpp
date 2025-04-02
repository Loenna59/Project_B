// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"

#include "BanimalsGameInstance.h"
#include "LobbyGameState.h"
#include "LobbyPlayerState.h"
#include "GameFramework/PlayerState.h"
#include "Online/CoreOnline.h"
#include "GameFramework/OnlineReplStructs.h"

ALobbyGameMode::ALobbyGameMode()
{
	GameStateClass = ALobbyGameState::StaticClass();
	PlayerStateClass = ALobbyPlayerState::StaticClass();
}

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	// 게임 인스턴스
	gi = Cast<UBanimalsGameInstance>(GetWorld()->GetGameInstance());

	// 플레이어 키값
	const FUniqueNetIdRepl& NetIdRepl = NewPlayer->GetPlayerState<APlayerState>()->GetUniqueId();
	if (NetIdRepl.IsValid())
	{
		TSharedPtr<const FUniqueNetId> NetId = NetIdRepl.GetUniqueNetId();
		Key = NetId->ToString(); // 요렇게!
		UE_LOG(LogTemp, Warning, TEXT("ALobbyGameMode::PostLogin: %s"), *Key);
	}

	// 팀을 배정하자
	FMapInfo* CurrentMapInfo = gi->GetCurrentMapInfo();
	teamMaxPlayers = CurrentMapInfo->TeamMaxPlayers;
	
	// 들어온 순서대로
	teamCnt = (playerIdx % teamMaxPlayers) + 1; // 레드(1), 블루(2), 옐로(3), 그린(4)
	ETeamType AssignedTeam = static_cast<ETeamType>(teamCnt);

	// 플레이어 저장
	FPlayerInfo NewPlayerInfo;
	NewPlayerInfo.PlayerID = playerIdx;
	NewPlayerInfo.Team = AssignedTeam;
	
	UE_LOG(LogTemp,Warning,TEXT("TeamMaxPlayers: %d"), teamMaxPlayers);
	UE_LOG(LogTemp,Warning,TEXT("playerIdx: %d"), playerIdx);
	UE_LOG(LogTemp,Warning,TEXT("teamCnt: %d"), AssignedTeam);

	gi->AddPlayerInfo(Key, NewPlayerInfo);

	// 멀티캐스트 해주자
	ALobbyGameState* gs = Cast<ALobbyGameState>(GetWorld()->GetGameState());
	if (gs)
	{
		UE_LOG(LogTemp, Warning, TEXT("GameStateClass is not Null"));

		FTimerHandle TimerHandle;
		TWeakObjectPtr<ALobbyGameState> WeakGameState = gs;
		TWeakObjectPtr<ALobbyGameMode> WeakThis = this;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([WeakGameState, WeakThis, &NewPlayerInfo]()
		{
			TMap<FString, FPlayerInfo>& InfoMap = WeakThis->gi->GetPlayerInfo();
			// for (변수 : 배열 및 자료구조) 범위 기반 탐색
			for (auto& it : InfoMap)
			{
				WeakGameState->MulticastRPC_UpdatePlayerTeam(it.Key, it.Value);
			}
		}), 1.0f, false);
	}
	else
	{
		UE_LOG(LogTemp,Error,TEXT("GameStateClass is NULL"));
	}
	
	playerIdx++;
}

