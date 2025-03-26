// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "Project_B/Maps/TeamMatchType.h"
#include "LuggagePlayerState.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_B_API ALuggagePlayerState : public APlayerState
{
	GENERATED_BODY()

private:
	ETeamType MyTeamType = ETeamType::None;
	
	bool bIsWin = false;
	
public:
	void SetTeamType(const ETeamType team) { MyTeamType = team; }
	ETeamType GetTeamType() const { return MyTeamType; }

	void SetIsWin(bool isWin) { bIsWin = isWin; }
	bool GetIsWin() { return bIsWin; }
};
