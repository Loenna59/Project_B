// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LuggageLobbyUI.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_B_API ULuggageLobbyUI : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	UPROPERTY()
	class UBanimalsGameInstance* gi;

	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Start;

	// 이미지 배열 만들기
	UPROPERTY(meta = (BindWidget))
	class UImage* red01_on;
	UPROPERTY(meta = (BindWidget))
	class UImage* red02_on;
	UPROPERTY(meta = (BindWidget))
	class UImage* red03_on;
	UPROPERTY(meta = (BindWidget))
	class UImage* red04_on;
	UPROPERTY(meta = (BindWidget))
	class UImage* blue01_on;
	UPROPERTY(meta = (BindWidget))
	class UImage* blue02_on;
	UPROPERTY(meta = (BindWidget))
	class UImage* blue03_on;
	UPROPERTY(meta = (BindWidget))
	class UImage* blue04_on;
	// 사람들어오면 UI 이미지 업데이트 하자
	void UpdateImage();

	// 버튼 누르면 러기지 게임 맵으로 이동하는것...
	UFUNCTION()
	void GameStart();
};
