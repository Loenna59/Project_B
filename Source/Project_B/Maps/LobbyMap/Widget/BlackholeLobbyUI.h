// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BlackholeLobbyUI.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_B_API UBlackholeLobbyUI : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	UPROPERTY()
	class UBanimalsGameInstance* gi;

	// 이미지 배열 만들기
	UPROPERTY(meta = (BindWidget))
	class UImage* red01_on;
	UPROPERTY(meta = (BindWidget))
	class UImage* red02_on;
	UPROPERTY(meta = (BindWidget))
	class UImage* blue01_on;
	UPROPERTY(meta = (BindWidget))
	class UImage* blue02_on;
	UPROPERTY(meta = (BindWidget))
	class UImage* yellow01_on;
	UPROPERTY(meta = (BindWidget))
	class UImage* yellow02_on;
	UPROPERTY(meta = (BindWidget))
	class UImage* green01_on;
	UPROPERTY(meta = (BindWidget))
	class UImage* green02_on;
	// 사람들어오면 UI 이미지 업데이트 하자
	void UpdateImage();

	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Start;

	// 버튼 누르면 블랙홀 게임 맵으로 이동하는것...
	UFUNCTION()
	void GameStart();
};
