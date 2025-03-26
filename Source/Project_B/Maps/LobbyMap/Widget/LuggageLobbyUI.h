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

	// 버튼 누르면 러기지 게임 맵으로 이동하는것...
	UFUNCTION()
	void GameStart();
};
