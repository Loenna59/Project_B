// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
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
};
