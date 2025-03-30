// Fill out your copyright notice in the Description page of Project Settings.


#include "BlackholeLobbyUI.h"

#include "Components/Button.h"
#include "Components/Image.h"
#include "Project_B/Maps/LobbyMap/BanimalsGameInstance.h"

void UBlackholeLobbyUI::NativeConstruct()
{
	Super::NativeConstruct();

	gi = Cast<UBanimalsGameInstance>(GetWorld()->GetGameInstance());
	Btn_Start->OnClicked.AddDynamic(this, &UBlackholeLobbyUI::GameStart);
}

void UBlackholeLobbyUI::UpdateImage()
{
	// 이미지 배열 저장
	TArray<UImage*> DefaultImages = { red01_on, blue01_on, yellow01_on, green01_on, red02_on, blue02_on, yellow02_on, green02_on };
	
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

void UBlackholeLobbyUI::GameStart()
{
	// Btn_Start->SetStyle(FButtonStyle::SetHovered())
	APlayerController* pc = GetWorld()->GetFirstPlayerController();
	if (pc->HasAuthority())
	{
		GetWorld()->ServerTravel(TEXT("/Game/Maps/BlackholeMap_01?listen"));
		UE_LOG(LogTemp, Warning, TEXT("블랙홀, 게임시작"));
	}
}
