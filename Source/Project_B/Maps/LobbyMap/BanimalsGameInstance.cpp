// Fill out your copyright notice in the Description page of Project Settings.


#include "BanimalsGameInstance.h"

#include "LobbyGameMode.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Online/OnlineSessionNames.h"
#include "Widget/BlackholeLobbyUI.h"
#include "Widget/LuggageLobbyUI.h"

void UBanimalsGameInstance::Init()
{
	Super::Init();

	// 현재 사용하는 서브시스템을 가져오자
	IOnlineSubsystem* subsys = IOnlineSubsystem::Get();
	if (subsys)
	{
		sessionInterface = subsys->GetSessionInterface();

		// 세션 생성 성공시 호출되는 함수 등록
		sessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UBanimalsGameInstance::OnCreateSessionComplete);
		// 세션 검색 성공시 호출되는 함수 등록
		sessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UBanimalsGameInstance::OnFindSessionComplete);
		// 세션 참여 성공시 호출되는 함수 등록
		sessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UBanimalsGameInstance::OnJoinSessionComplete);
	}

	InitializeMapInfo();
}

void UBanimalsGameInstance::GetLifetimeReplicatedProps(
	TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
}

void UBanimalsGameInstance::CreateLobbySession(FString displayName, int32 playerCount)
{
	// 세션을 만들기 위한 옵션을 설정
	FOnlineSessionSettings sessionSettings;

	// Lan 사용 여부
	FName subsystemName = IOnlineSubsystem::Get()->GetSubsystemName();
	UE_LOG(LogTemp, Warning, TEXT("서브시스템 이름: %s"), *subsystemName.ToString())
	sessionSettings.bIsLANMatch = subsystemName.IsEqual(FName(TEXT("NULL")));

	// 로비 사용할지 여부
	sessionSettings.bUseLobbiesIfAvailable = true;
	// 위와 세트임
	// 친구 상태를 확인할 수 있는지 (게임중/로그아웃 등등 공개할건지) 여부
	sessionSettings.bUsesPresence = true;
	// 세션 검색을 허용할 지 여부
	sessionSettings.bShouldAdvertise = true;
	// 세션 최대 인원 설정
	sessionSettings.NumPublicConnections = playerCount;
	/*// 커스텀 정보 (세션이름)
	sessionSettings.Set(FName(TEXT("DP_NAME")), displayName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);*/

	// 세션 생성
	sessionInterface->CreateSession(0,FName(displayName),sessionSettings);
}

void UBanimalsGameInstance::OnCreateSessionComplete(FName sessionName, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s] 세션 생성 성공"), *sessionName.ToString());
		APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
		
		// 서버가 팀 배정 맵으로 이동!
		// TODO: 로비맵으로 이동할것이다
		if (CurrentMapID == 0)
		{
			GetWorld()->ServerTravel(TEXT("/Game/Maps/Lobby/GameLobbyMap?listen"));
			UE_LOG(LogTemp, Warning, TEXT("듀오 모드. 블랙홀 로비로"));
		}
		if (CurrentMapID == 1)
		{
			GetWorld()->ServerTravel(TEXT("/Game/Maps/Lobby/GameLobbyMap?listen"));
			UE_LOG(LogTemp, Warning, TEXT("팀 모드. 러기지 로비로"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s] 세션 생성 실패"), *sessionName.ToString());
	}
}

void UBanimalsGameInstance::FindOtherSession()
{
	// sessionSearch 만들자
	sessionSearch = MakeShared<FOnlineSessionSearch>();

	// Lan 사용 여부
	FName subsystemName = IOnlineSubsystem::Get()->GetSubsystemName();
	UE_LOG(LogTemp, Warning, TEXT("서브시스템 이름: %s"), *subsystemName.ToString())
	sessionSearch->bIsLanQuery = subsystemName.IsEqual(FName(TEXT("NULL")));

	// 어떤 옵션을 기준으로 검색
	sessionSearch->QuerySettings.Set(SEARCH_LOBBIES, true, EOnlineComparisonOp::Equals);

	// 위 설정들을 가지고 세션 검색해주세요
	sessionInterface->FindSessions(0, sessionSearch.ToSharedRef());
}

void UBanimalsGameInstance::OnFindSessionComplete(bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		UE_LOG(LogTemp,Warning,TEXT("세션 검색 성공!"));
		// 검색된 세션 결과들
		auto results = sessionSearch->SearchResults;
		for (int32 i=0; i<results.Num(); i++)
		{
			FString displayName;
			results[i].Session.SessionSettings.Get(FName(TEXT("DP_NAME")), displayName);
			UE_LOG(LogTemp,Warning,TEXT("세션 - %d, 이름: %s"), i, *displayName);
		}
	}
	else
	{
		UE_LOG(LogTemp,Warning,TEXT("세션 검색 실패"));
	}
}

void UBanimalsGameInstance::JoinOtherSession()
{
	// 검색된 세션 결과들
	auto results = sessionSearch->SearchResults;
	if (results.Num() == 0) return;

	// 세션 이름을 가져오자 (일단 0번째)
	FString displayName;
	// 5.5 이슈 해결 (이 값이 자동으로 false되니까 다시 변환해주기)
	results[0].Session.SessionSettings.bUsesPresence = true;
	results[0].Session.SessionSettings.bUseLobbiesIfAvailable = true;
	
	results[0].Session.SessionSettings.Get(FName(TEXT("DP_NAME")), displayName);

	// 세션참여
	sessionInterface->JoinSession(0, FName(displayName), results[0]);
}

void UBanimalsGameInstance::OnJoinSessionComplete(FName sessionName, EOnJoinSessionCompleteResult::Type result)
{
	// 만약에 참여 성공했다면
	if (result == EOnJoinSessionCompleteResult::Success)
	{
		// 서버가 만들어 놓은 세션 url얻어오자
		FString url;
		sessionInterface->GetResolvedConnectString(sessionName, url);
		// 서버가 있는 맵으로 이동하자
		APlayerController* pc = GetWorld()->GetFirstPlayerController();
		pc->ClientTravel(url, TRAVEL_Absolute);
	}
}

void UBanimalsGameInstance::InitializeMapInfo()
{
	// 블랙홀 맵 정보
	FMapInfo BlackholeMap;
	BlackholeMap.MapID = 0;
	BlackholeMap.TeamMaxPlayers = 4;
	BlackholeMap.MaxPlayers = 8;
	BlackholeMap.LoadingWidget = nullptr;

	// 공항 맵 정보
	FMapInfo LuggageMap;
	LuggageMap.MapID = 1;
	LuggageMap.TeamMaxPlayers = 2;
	LuggageMap.MaxPlayers = 8;
	LuggageMap.LoadingWidget = nullptr;

	// 맵 정보 추가
	MapInfoList.Add(BlackholeMap.MapID, BlackholeMap);
	MapInfoList.Add(LuggageMap.MapID, LuggageMap);
}
