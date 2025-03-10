// Fill out your copyright notice in the Description page of Project Settings.


#include "LuggageChaosGameMode.h"

#include "Project_B/Utilities/LogMacro.h"

void ALuggageChaosGameMode::AddScoreRed(const uint8 point)
{
	RedPoint += point;
	LOG_SCREEN("레드팀 스코어: %d", RedPoint);
}

void ALuggageChaosGameMode::AddScoreBlue(const uint8 point)
{
	BluePoint += point;
	LOG_SCREEN("블루팀 스코어: %d", BluePoint);
}
