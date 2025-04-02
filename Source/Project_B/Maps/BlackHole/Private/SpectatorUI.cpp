// Fill out your copyright notice in the Description page of Project Settings.


#include "Project_B/Maps/BlackHole/Public/SpectatorUI.h"

#include "Components/Button.h"

void USpectatorUI::NativeConstruct()
{
	Super::NativeConstruct();

	coinCount = 3;
	btn_banana->OnClicked.AddDynamic(this, &USpectatorUI::Banana_Clicked);
	btn_poo->OnClicked.AddDynamic(this, &USpectatorUI::Poo_Clicked);
	btn_bomb->OnClicked.AddDynamic(this, &USpectatorUI::Bomb_Clicked);
}

void USpectatorUI::Banana_Clicked()
{
	// 코인이 3개이상일때만 실행
	if (coinCount >= 3)
	{
		UE_LOG(LogTemp, Display, TEXT("Banana_Clicked"));
		// 관전자의 스폰 아이템은 바나나
		// 관전자 스폰 함수 실행
		coinCount -= 3;
	}
}

void USpectatorUI::Poo_Clicked()
{
	// 4개이상
	if (coinCount >= 4)
	{
		coinCount -= 4;
	}
}

void USpectatorUI::Bomb_Clicked()
{
	if (coinCount >= 5)
	{

		coinCount -= 5;
	}
}

void USpectatorUI::UpdateItemImages()
{
	
}
