// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Project_B/Maps/BanimalsType.h" // 팀정보
#include "BanimalsGameInstance.generated.h"

// 세션 검색 완료시 호출되는 함수를 등록하는 델리게이트
DECLARE_DELEGATE_TwoParams(FFindComplete, int32, FString);


UCLASS()
class PROJECT_B_API UBanimalsGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	virtual void Init() override;

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
	void JoinOtherSession(int32 sessionIdx);
	void OnJoinSessionComplete(FName sessionName, EOnJoinSessionCompleteResult::Type result);
	
public:
	// 세션의 모든 처리를 진행해주는 객체
	IOnlineSessionPtr sessionInterface;

	// 세션 검색할 때 사용하는 객체
	TSharedPtr<FOnlineSessionSearch> sessionSearch;
	
	// 세션 검색완료시 호출되는 델리게이트
	FFindComplete OnFindComplete;

private:
	FString NetID;
	TMap<FString, FPlayerInfo> PlayerMap;

public:
	void SetPlayerInfo(const TMap<FString, FPlayerInfo> info) { PlayerMap = info; }
	void AddPlayerInfo(const FString& PlayerKey, const FPlayerInfo& PlayerInfo);
	TMap<FString, FPlayerInfo>& GetPlayerInfo() { return PlayerMap; }
	
public:
	// 맵 정보 초기화
	void InitializeMapInfo();
	
	// 맵 정보 저장
	UPROPERTY()
	TMap<int32, FMapInfo> MapInfoList;
	// 현재 선택된 맵 ID
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 CurrentMapID;
	void SetCurrentMapInfo(int32 MapID) { CurrentMapID = MapID; }
	FMapInfo* GetCurrentMapInfo() {	return MapInfoList.Find(CurrentMapID); }
};

