// Fill out your copyright notice in the Description page of Project Settings.


#include "LuggageChaosGameState.h"

#include "Project_B/Utilities/LogMacro.h"


void ALuggageChaosGameState::AddScore(ETeamType team, const uint8 point)
{
	if (team == ETeamType::Blue)
	{
		BluePoint += point;
		LOG_SCREEN("블루팀 스코어: %d", BluePoint);
		if (BluePoint >= MaxPoint)
		{
			Win(ETeamType::Blue);
		}
	}
	else
	{
		RedPoint += point;
		LOG_SCREEN("레드팀 스코어: %d", RedPoint);
		if (RedPoint >= MaxPoint)
		{
			Win(ETeamType::Red);
		}
	}
}

void ALuggageChaosGameState::Win(ETeamType team)
{
	if (team == ETeamType::Blue)
	{
		LOG_SCREEN("블루팀 승리");
	}
	else
	{
		LOG_SCREEN("레드팀 승리");
	}
}