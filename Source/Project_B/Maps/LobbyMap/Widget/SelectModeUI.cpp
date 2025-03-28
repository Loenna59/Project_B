// Fill out your copyright notice in the Description page of Project Settings.


#include "SelectModeUI.h"

#include "Components/Button.h"
#include "Project_B/Maps/LobbyMap/BanimalsGameInstance.h"

void USelectModeUI::NativeConstruct()
{
	Super::NativeConstruct();
	
	gi = Cast<UBanimalsGameInstance>(GetWorld()->GetGameInstance());
	Btn_DuoBlackHole->OnClicked.AddDynamic(this, &USelectModeUI::CreateBlackholeSession);
	Btn_TeamLuggage->OnClicked.AddDynamic(this, &USelectModeUI::CreateLuggageSession);
}

void USelectModeUI::CreateBlackholeSession()
{
	// 대기방으로 이동하자
	gi->SetCurrentMapInfo(0);

	// 생성
	gi->CreateLobbySession(FString(TEXT("Blackhole")), 9);
	RemoveFromParent();
}

void USelectModeUI::CreateLuggageSession()
{
	// 대기방으로 이동하자
	gi->SetCurrentMapInfo(1);
	
	// 생성
	gi->CreateLobbySession(FString(TEXT("Luggage")), 9);
	RemoveFromParent();
}
