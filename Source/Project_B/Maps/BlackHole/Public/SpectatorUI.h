// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SpectatorUI.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_B_API USpectatorUI : public UUserWidget
{
	GENERATED_BODY()
public:
	// 아이템 버튼
	UPROPERTY(meta = (BindWidget))
	class UButton* btn_banana;
	UPROPERTY(meta = (BindWidget))
	class UButton* btn_poo;
	UPROPERTY(meta = (BindWidget))
	class UButton* btn_bomb;
	
	// 코인이미지
	UPROPERTY(meta = (BindWidget))
	class UImage* coin1;
	UPROPERTY(meta = (BindWidget))
	class UImage* coin2;
	UPROPERTY(meta = (BindWidget))
	class UImage* coin3;
	UPROPERTY(meta = (BindWidget))
	class UImage* coin4;
	UPROPERTY(meta = (BindWidget))
	class UImage* coin5;

	// 관전중
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_SpectatorMode;

	// 승리 중인 팀이름
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TeamName;
};
