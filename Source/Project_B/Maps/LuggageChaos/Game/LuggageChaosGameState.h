// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Project_B/Maps/BanimalsType.h"
#include "LuggageChaosGameState.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_B_API ALuggageChaosGameState : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ReadyTime = 2.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float GameTime = 6.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SlowTime = 0.2f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 MaxPoint = 24;

	ETeamType WinnerTeam = ETeamType::None;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class ULuggageScoreWidget> ScoreWidgetClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UGameEndWidget> GameEndWidgetClass;

private:
	uint8 RedPoint = 0;
	uint8 BluePoint = 0;

	FTimerHandle GameTimerHandle;
	
	UPROPERTY()
	ULuggageScoreWidget* ScoreWidget;
	UPROPERTY()
	UGameEndWidget* GameEndWidget;


protected:
	virtual void BeginPlay() override;
	
public:
	/**서버만 실행*/
	void GameStart();
	
	/**모든 클라이언트에서 실행되어야 함*/
	void GameEnd();

	/**서버에서만 실행*/
	UFUNCTION(NetMulticast, Reliable)
	void Net_GameEnd();

	/**모든 클라이언트에서 실행되어야 함*/
	void AddScore(ETeamType team ,const uint8 point);

	/**모든 클라이언트에서 실행되어야 함*/
	void Win(ETeamType team = ETeamType::None);
	
	/**서버만 실행*/
	void TimeOut();
	
	UFUNCTION(NetMulticast, Reliable)
	void Net_JudgeWinner();

	UFUNCTION(NetMulticast, Reliable)
	void Net_InitUI();
};
