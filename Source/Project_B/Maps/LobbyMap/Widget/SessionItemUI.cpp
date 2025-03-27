// Fill out your copyright notice in the Description page of Project Settings.


#include "SessionItemUI.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Project_B/Maps/LobbyMap/BanimalsGameInstance.h"

void USessionItemUI::NativeConstruct()
{
	Super::NativeConstruct();

	Btn_JoinSession->OnClicked.AddDynamic(this,&USessionItemUI::Btn_JoinSession_Clicked);
}

void USessionItemUI::Btn_JoinSession_Clicked()
{
	UBanimalsGameInstance* gi = Cast<UBanimalsGameInstance>(GetWorld()->GetGameInstance());
	gi->JoinOtherSession(roomIdx);
}

void USessionItemUI::SetInfo(int32 idx, FString info)
{
	// 현재 몇번째 방인지?
	roomIdx = idx;
	// 방 정보 받기 (업데이트)
	Text_SessionName->SetText(FText::FromString(info));
}
