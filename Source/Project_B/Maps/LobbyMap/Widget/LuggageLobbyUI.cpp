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
	APlayerController* pc = GetWorld()->GetFirstPlayerController();
	if (pc->HasAuthority())
	{
		GetWorld()->ServerTravel(TEXT("/Game/Maps/LV_LuggageChaos_01?listen"));
		UE_LOG(LogTemp, Warning, TEXT("러기지, 게임시작"));
	}
}
