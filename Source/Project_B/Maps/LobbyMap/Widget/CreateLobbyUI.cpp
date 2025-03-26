// Fill out your copyright notice in the Description page of Project Settings.


#include "CreateLobbyUI.h"
#include "Components/Button.h"
#include "Project_B/Maps/LobbyMap/BanimalsGameInstance.h"

void UCreateLobbyUI::NativeConstruct()
{
	Super::NativeConstruct();
	
	gi = Cast<UBanimalsGameInstance>(GetWorld()->GetGameInstance());
	Btn_JoinSession->OnClicked.AddDynamic(this, &UCreateLobbyUI::JoinSession);
}


void UCreateLobbyUI::JoinSession()
{
	// 만들어진 세션에 참여하고 싶은데요...
	// gi->JoinOtherSession();
}
