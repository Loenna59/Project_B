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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Timer")
	float ReadyTime = 2.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Timer")
	float GameTime = 6.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Timer")
	float SlowTime = 0.2f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Timer")
	float EndTime = 1.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Timer")
	float RespawnTime = 3.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category ="Game")
	uint8 MaxPoint = 24;

	ETeamType WinnerTeam = ETeamType::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Class | WinPrize")
	TSubclassOf<class AWinnerPrize> WinnerPrizeClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Class | UI")
	TSubclassOf<class ULuggageScoreWidget> ScoreWidgetClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Class | UI")
	TSubclassOf<class UGameEndWidget> GameEndWidgetClass;

private:
	uint8 RedPoint = 0;
	uint8 BluePoint = 0;

	FTimerHandle GameTimerHandle;

	float TrueEndTime = 0.0f;
	
	UPROPERTY()
	ULuggageScoreWidget* ScoreWidget;
	UPROPERTY()
	UGameEndWidget* GameEndWidget;


protected:
	virtual void BeginPlay() override;
	
public:
	/**모든 클라이언트에서 실행*/
	void AddScore(ETeamType team ,const uint8 point);

protected:
	/**서버만 실행*/
	void GameStart();
	
	/**모든 클라이언트에서 실행*/
	void GameEnd();
	/**서버에서만 실행*/
	UFUNCTION(NetMulticast, Reliable)
	void Net_GameEnd();

	/**모든 클라이언트에서 실행*/
	void Win(ETeamType team = ETeamType::None);
	void AddWinPrize(APlayerController* pc);
	
	/**서버만 실행*/
	void TimeOut();

	/**서버만 실행*/
	void ChangeLevelPodium();
	
	UFUNCTION(NetMulticast, Reliable)
	void Net_JudgeWinner();

	UFUNCTION(NetMulticast, Reliable)
	void Net_InitUI();
};
