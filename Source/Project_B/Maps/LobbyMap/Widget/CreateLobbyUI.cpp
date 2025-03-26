// Fill out your copyright notice in the Description page of Project Settings.


#include "CreateLobbyUI.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Project_B/Maps/LobbyMap/BanimalsGameInstance.h"

void UCreateLobbyUI::NativeConstruct()
{
	Super::NativeConstruct();

	// 마우스 활성화
	GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(true);
	gi = Cast<UBanimalsGameInstance>(GetWorld()->GetGameInstance());

	Btn_MainCustomGame->OnClicked.AddDynamic(this, &UCreateLobbyUI::Btn_MainCustomGame_Clicked);
	Btn_GoFirst->OnClicked.AddDynamic(this, &UCreateLobbyUI::Btn_GoFirst_Clicked);

	Btn_FindSession->OnClicked.AddDynamic(this, &UCreateLobbyUI::Btn_FindSession_Clicked);
}


void UCreateLobbyUI::Btn_MainCustomGame_Clicked()
{
	WidgetSwitcher->SetActiveWidgetIndex(1);
}

void UCreateLobbyUI::Btn_FindSession_Clicked()
{
}

void UCreateLobbyUI::Btn_GoFirst_Clicked()
{
	WidgetSwitcher->SetActiveWidgetIndex(0);
}
