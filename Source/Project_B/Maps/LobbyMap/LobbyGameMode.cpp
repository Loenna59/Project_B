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
	
	playerIdx = 0;
	
	// 게임 인스턴스
	gi = Cast<UBanimalsGameInstance>(GetWorld()->GetGameInstance());

	if (gi->bIsBlackholeMode)
	{
		teamIndex = 4;
	}
	else
	{
		teamIndex = 2;
	}
	
	// 들어온 순서대로 팀을 배정받자
	if (gi)
	{
		// 로비에서 맵인식해서 2,4 구분
		teamCnt = playerIdx % teamIndex; // 레드(1), 블루(2), 옐로(3), 그린(4)
		ETeamType AssignedTeam = static_cast<ETeamType>(teamCnt);

		// 게임 모드는 서버에만 존재하므로 클라이언트들은 이 값을 모름
		// 값 덮어 씌워짐
		gi->SetTeamType(AssignedTeam);
		
		playerIdx++;
	}
}


