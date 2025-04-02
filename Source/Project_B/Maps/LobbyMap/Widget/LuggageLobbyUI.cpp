// Fill out your copyright notice in the Description page of Project Settings.


#include "LuggageLobbyUI.h"

#include "Components/Button.h"
#include "Components/Image.h"
#include "Project_B/Maps/LobbyMap/BanimalsGameInstance.h"

void ULuggageLobbyUI::NativeConstruct()
{
	Super::NativeConstruct();

	gi = Cast<UBanimalsGameInstance>(GetWorld()->GetGameInstance());
	Btn_Start->OnClicked.AddDynamic(this, &ULuggageLobbyUI::GameStart);
}

void ULuggageLobbyUI::UpdateImage()
{
	if (!gi)
	{
		UE_LOG(LogTemp, Error, TEXT("GameInstance is not valid in UpdateImage"));
		return;
	}
	
	// 이미지 배열 저장
	TArray<UImage*> DefaultImages = { red01_on, blue01_on, red02_on, blue02_on, red03_on, blue03_on, red04_on, blue04_on };
	
	// APlayerController* player = GetWorld()->GetFirstPlayerController(); // 내꺼만 들고옴
	TMap<FString, FPlayerInfo> info = gi->GetPlayerInfo();
	FString Key;
	
	for (int i = 0; i < DefaultImages.Num(); i++)
	{
		for (auto& it : info)
		{
			UE_LOG(LogTemp, Warning, TEXT("Stored Key : %s"), *it.Key);
			if (it.Value.PlayerID == i)
			{
				// 플레이어 인덱스와 같은 이미지를 보이게 하자
				DefaultImages[i]->SetRenderOpacity(1);
			}
		}
	}
}

void ULuggageLobbyUI::GameStart()
{
	APlayerController* pc = GetWorld()->GetFirstPlayerController();
	if (pc->HasAuthority())
	{
		GetWorld()->ServerTravel(TEXT("/Game/Maps/LV_LuggageChaos_01?listen"));
		UE_LOG(LogTemp, Warning, TEXT("러기지, 게임시작"));
	}
}
