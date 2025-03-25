// Fill out your copyright notice in the Description page of Project Settings.


#include "BlackholeLobbyUI.h"

#include "Components/Button.h"
#include "Project_B/Maps/LobbyMap/BanimalsGameInstance.h"

void UBlackholeLobbyUI::NativeConstruct()
{
	Super::NativeConstruct();

	gi = Cast<UBanimalsGameInstance>(GetWorld()->GetGameInstance());
	Btn_Start->OnClicked.AddDynamic(this, &UBlackholeLobbyUI::GameStart);
}

void UBlackholeLobbyUI::GameStart()
{
	gi->bClick = true;
}
