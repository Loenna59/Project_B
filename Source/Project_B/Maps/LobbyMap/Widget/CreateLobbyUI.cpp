// Fill out your copyright notice in the Description page of Project Settings.


#include "CreateLobbyUI.h"

#include "SessionItemUI.h"
#include "Components/Button.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
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
	gi->OnFindComplete.BindUObject(this, &UCreateLobbyUI::OnFindComplete);
}


void UCreateLobbyUI::Btn_MainCustomGame_Clicked()
{
	WidgetSwitcher->SetActiveWidgetIndex(1);
}

void UCreateLobbyUI::Btn_FindSession_Clicked()
{
	// 스크롤 룸리스트 자식들 다 지우자
	ScrollBox->ClearChildren();
	// 그 다음에 검색을 시작
	gi->FindOtherSession();

	// 찾는 도중엔 버튼을 비활성화
	// 검색 버튼 문구 바꿔주고
	TextBtn_FindSession->SetText(FText::FromString("Search..."));
	// 검색 버튼을 비활성화
	Btn_FindSession->SetIsEnabled(false);
}

void UCreateLobbyUI::Btn_GoFirst_Clicked()
{
	WidgetSwitcher->SetActiveWidgetIndex(0);
}

void UCreateLobbyUI::OnFindComplete(int32 idx, FString info)
{
	// 만약에 idx가 -1이면
	if (idx==-1)
	{
		// 검색 버튼 내용 다시 find로
		TextBtn_FindSession->SetText(FText::FromString("Find"));
		// 검색 버튼을 활성화
		Btn_FindSession->SetIsEnabled(true);
	}
	// 그렇지 않으면
	else
	{
		// SessionItem을 하나 만들자
		USessionItemUI* item = CreateWidget<USessionItemUI>(GetWorld(), sessionItemFactory);
		// 만들어진 SessionItem을 Scroll_RoomList에 자식으로!
		ScrollBox->AddChild(item);
		// 만들어진 SessionItem의 Text 내용을 변경하고, idx 전달하자
		item->SetInfo(idx, info);
	}
}
