// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Project_B/Maps/BanimalsType.h"
#include "LobbyGameState.generated.h"

class ULuggageLobbyUI;
class UBlackholeLobbyUI;
/**
 * 
 */

UCLASS()
class PROJECT_B_API ALobbyGameState : public AGameStateBase
{
	GENERATED_BODY()
public:
	ALobbyGameState();
	virtual void BeginPlay() override;

	UPROPERTY()
	class UBanimalsGameInstance* gi;

	// 위젯 받아오기 (블랙홀)
	UPROPERTY()
	TSubclassOf<UBlackholeLobbyUI> BlackholeLobbyWidgetClass;
	UPROPERTY()
	UBlackholeLobbyUI* BlackholeLobbyWidget;
	// 러기지
	UPROPERTY()
	TSubclassOf<ULuggageLobbyUI> LuggageLobbyWidgetClass;
	UPROPERTY()
	ULuggageLobbyUI* LuggageLobbyWidget;

	// 플레이어가 들어올 때마다 정보를 보내줄 것이다
	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_UpdatePlayerTeam(const FString& PlayerKey, const FPlayerInfo& PlayerInfo);
};
