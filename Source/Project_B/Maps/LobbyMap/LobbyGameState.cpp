// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameState.h"

#include "BanimalsGameInstance.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerState.h"
#include "Widget/BlackholeLobbyUI.h"
#include "Widget/LuggageLobbyUI.h"

ALobbyGameState::ALobbyGameState()
{
	bReplicates = true;
	
	ConstructorHelpers::FClassFinder<UBlackholeLobbyUI> TempBHUI(TEXT("/Game/Maps/Lobby/UI/WBP_BlackholeLobby.WBP_BlackholeLobby_C"));
	if (TempBHUI.Succeeded())
	{
		BlackholeLobbyWidgetClass = TempBHUI.Class;
	}
	ConstructorHelpers::FClassFinder<ULuggageLobbyUI> TempLGUI(TEXT("/Game/Maps/Lobby/UI/WBP_LuggageLobby.WBP_LuggageLobby_C"));
	if (TempLGUI.Succeeded())
	{
		LuggageLobbyWidgetClass = TempLGUI.Class;
	}
}

void ALobbyGameState::BeginPlay()
{
	Super::BeginPlay();

	// 게임 인스턴스
	gi = Cast<UBanimalsGameInstance>(GetWorld()->GetGameInstance());
	UE_LOG(LogTemp, Warning, TEXT("게임스테이트 실행"));
	
	if (gi->CurrentMapID == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("블랙홀 변수 확인"));
		if (BlackholeLobbyWidgetClass) // 위젯 클래스가 설정되었는지 확인
		{
			// 위젯 생성
			BlackholeLobbyWidget = CreateWidget<UBlackholeLobbyUI>(GetWorld(), BlackholeLobbyWidgetClass);

			if (BlackholeLobbyWidget)
			{
				BlackholeLobbyWidget->AddToViewport(); // UI 화면에 추가
				UE_LOG(LogTemp, Warning, TEXT("블랙홀 UI add"));
			}
		}
	}
	if (gi->CurrentMapID == 1)
	{
		if (LuggageLobbyWidgetClass) // 위젯 클래스가 설정되었는지 확인
		{
			// 위젯 생성
			LuggageLobbyWidget = CreateWidget<ULuggageLobbyUI>(GetWorld(), LuggageLobbyWidgetClass);

			if (LuggageLobbyWidget)
			{
				LuggageLobbyWidget->AddToViewport(); // UI 화면에 추가
			}
		}
	}
}

void ALobbyGameState::MulticastRPC_UpdatePlayerTeam_Implementation(const FString& PlayerKey,
                                                                   const FPlayerInfo& PlayerInfo)
{
	UE_LOG(LogTemp, Warning, TEXT("HELLO"));
	if (gi && !HasAuthority())
	{
		gi->AddPlayerInfo(PlayerKey, PlayerInfo);
		TMap<FString, FPlayerInfo> info = gi->GetPlayerInfo();
		for (auto& it : info)
		{
			UE_LOG(LogTemp, Warning, TEXT("%s, %d"), *it.Key, it.Value.Team);
		}
	}
}
