// Fill out your copyright notice in the Description page of Project Settings.


#include "LuggageLobbyUI.h"

#include "Components/Button.h"
#include "Project_B/Maps/LobbyMap/BanimalsGameInstance.h"

void ULuggageLobbyUI::NativeConstruct()
{
	Super::NativeConstruct();

	gi = Cast<UBanimalsGameInstance>(GetWorld()->GetGameInstance());
	Btn_Start->OnClicked.AddDynamic(this, &ULuggageLobbyUI::GameStart);
}

void ULuggageLobbyUI::GameStart()
{
	gi->bClick = true;
}
