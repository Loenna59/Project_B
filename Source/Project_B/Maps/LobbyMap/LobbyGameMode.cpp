// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"

#include "BanimalsGameInstance.h"
#include "LobbyGameState.h"
#include "GameFramework/PlayerState.h"
#include "Online/CoreOnline.h"
#include "GameFramework/OnlineReplStructs.h"

ALobbyGameMode::ALobbyGameMode()
{
	GameStateClass = ALobbyGameState::StaticClass();
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
	}

	// 팀을 배정하자
	FMapInfo* CurrentMapInfo = gi->GetCurrentMapInfo();
	teamMaxPlayers = CurrentMapInfo->TeamMaxPlayers;
	UE_LOG(LogTemp,Warning,TEXT("TeamMaxPlayers: %d"), teamMaxPlayers);
	
	// 들어온 순서대로
	teamCnt = (playerIdx % teamMaxPlayers) + 1; // 레드(1), 블루(2), 옐로(3), 그린(4)
	ETeamType AssignedTeam = static_cast<ETeamType>(teamCnt);
	UE_LOG(LogTemp,Warning,TEXT("teamCnt: %d"), teamCnt);

	// 플레이어 저장
	FPlayerInfo NewPlayerInfo;
	NewPlayerInfo.PlayerID = playerIdx;
	NewPlayerInfo.Team = AssignedTeam;
	
	PlayerMap.Add(Key, NewPlayerInfo);
	
	playerIdx++;
}
