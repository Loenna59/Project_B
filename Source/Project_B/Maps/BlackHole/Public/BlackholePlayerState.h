// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "BlackholePlayerState.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_B_API ABlackholePlayerState : public APlayerState
{
	GENERATED_BODY()
public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	// 플레이어의 팀 정보를 나누자
	UPROPERTY(ReplicatedUsing=OnRep_TeamID)
	int32 TeamID;
	// 팀 변경 함수
	UFUNCTION()
	void SetTeam(int32 NewTeamID);
	// 클라이언트에서 팀 변경 감지
	UFUNCTION()
	void OnRep_TeamID(); 
};
