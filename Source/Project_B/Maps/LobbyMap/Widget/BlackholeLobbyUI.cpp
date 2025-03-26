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
	APlayerController* pc = GetWorld()->GetFirstPlayerController();
	if (pc->HasAuthority())
	{
		GetWorld()->ServerTravel(TEXT("/Game/Maps/BlackholeMap_01?listen"));
		UE_LOG(LogTemp, Warning, TEXT("블랙홀, 게임시작"));
	}
}
