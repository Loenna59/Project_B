// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"

#include "BanimalsGameInstance.h"
#include "Net/UnrealNetwork.h"

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	
	playerIdx = 0;
	
	// 게임 인스턴스
	gi = Cast<UBanimalsGameInstance>(GetWorld()->GetGameInstance());
	// 들어온 순서대로 팀을 배정받자
	if (gi)
	{
		// 로비에서 맵인식해서 2,4 구분
		teamCnt = playerIdx % teamIndex; // 레드(1), 블루(2), 옐로(3), 그린(4)
		ETeamType AssignedTeam = static_cast<ETeamType>(teamCnt);
		gi->SetTeamType(AssignedTeam);
		
		playerIdx++;
	}
}
