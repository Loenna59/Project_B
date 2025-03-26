// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"

#include "BanimalsGameInstance.h"
#include "LobbyGameState.h"
#include "Net/UnrealNetwork.h"

ALobbyGameMode::ALobbyGameMode()
{
	GameStateClass = ALobbyGameState::StaticClass();
}

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	// 게임 인스턴스
	gi = Cast<UBanimalsGameInstance>(GetWorld()->GetGameInstance());

	// 모드 결정
	teamIndex = gi->bIsBlackholeMode ? 4 : 2;
	// 들어온 순서대로 팀을 배정받자
	teamCnt = (playerIdx % teamIndex) + 1; // 레드(1), 블루(2), 옐로(3), 그린(4)
	ETeamType AssignedTeam = static_cast<ETeamType>(teamCnt);
	

	// GameInstance에도 저장 (서버에서도 정보 유지) gi->SetPlayerTeam(playerIdx, AssignedTeam);
	
	playerIdx++;

}


