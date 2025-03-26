// Fill out your copyright notice in the Description page of Project Settings.


#include "SessionItemUI.h"

#include "Project_B/Maps/LobbyMap/BanimalsGameInstance.h"

void USessionItemUI::NativeConstruct()
{
	Super::NativeConstruct();
	
}

void USessionItemUI::Btn_JoinSession_Clicked()
{
	UBanimalsGameInstance* gi = Cast<UBanimalsGameInstance>(GetWorld()->GetGameInstance());
	// gi->JoinOtherSession(roomIdx);
}
