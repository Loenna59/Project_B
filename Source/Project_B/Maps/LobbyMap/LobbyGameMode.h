// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Project_B/Maps/BanimalsType.h"
#include "Widget/BlackholeLobbyUI.h"
#include "Widget/LuggageLobbyUI.h"
#include "LobbyGameMode.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_B_API ALobbyGameMode : public AGameModeBase
{
	GENERATED_BODY()
public:
	ALobbyGameMode();
	// 서버 게임 세션에 접속하면 자동 호출
	virtual void PostLogin(APlayerController* NewPlayer) override;

public:
	int32 playerIdx = 0;
	int32 teamMaxPlayers = 0;
	int32 teamCnt = 0;

	UPROPERTY()
	class UBanimalsGameInstance* gi;

	// 플레이어 정보를 저장
	FString Key;
	TMap<FString, FPlayerInfo> PlayerMap;
};
