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
	gi->SetBlackholeMode(true);

	// 생성
	gi->CreateLobbySession(FString(TEXT("Blackhole")), 8);
	RemoveFromParent();
}

void USelectModeUI::CreateLuggageSession()
{
	// 대기방으로 이동하자
	gi->SetBlackholeMode(false);
	
	// 생성
	gi->CreateLobbySession(FString(TEXT("Luggage")), 8);
	RemoveFromParent();
}
