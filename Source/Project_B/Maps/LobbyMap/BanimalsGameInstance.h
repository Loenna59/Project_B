// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Project_B/Maps/BanimalsType.h" // 팀정보
#include "BanimalsGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_B_API UBanimalsGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	virtual void Init() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// 세션 생성 관련
	UFUNCTION(BlueprintCallable)
	void CreateLobbySession(FString displayName, int32 playerCount);
	void OnCreateSessionComplete(FName sessionName, bool bWasSuccessful);

	// 세션 조회 관련
	UFUNCTION(BlueprintCallable)
	void FindOtherSession();
	UFUNCTION(BlueprintCallable)
	void OnFindSessionComplete(bool bWasSuccessful);

	// 세션 참여 관련
	UFUNCTION(BlueprintCallable)
	void JoinOtherSession();
	void OnJoinSessionComplete(FName sessionName, EOnJoinSessionCompleteResult::Type result);
	
public:
	// 세션의 모든 처리를 진행해주는 객체
	IOnlineSessionPtr sessionInterface;

	// 세션 검색할 때 사용하는 객체
	TSharedPtr<FOnlineSessionSearch> sessionSearch;

	
public:
	// 플레이어 팀 정보 관련
	UPROPERTY(Replicated)
	int32 TeamID;

	// 블랙홀인지 러기지인지
	// 블랙홀과 러기지를 구분할 변수
	bool bIsBlackholeMode = false;
	UFUNCTION(BlueprintCallable)
	void SetBlackholeMode(bool bEnabled) { bIsBlackholeMode = bEnabled; }
	// 버튼을 눌렀다면 맵으로 이동하자
	bool bClick = false;

	
private:
	int32 MyPlayerID;
	TArray<FPlayerInfo> PlayerList;
	
public:
	void SetPlayerInfo(const TArray<FPlayerInfo> info) { PlayerList = info; }
	void GetPlayerInfo(TArray<FPlayerInfo>& info) { info = PlayerList; }

};

