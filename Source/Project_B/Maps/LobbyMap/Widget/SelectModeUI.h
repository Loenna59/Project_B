// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SelectModeUI.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_B_API USelectModeUI : public UUserWidget
{
	GENERATED_BODY()
	
	// 모드를 고르고 세션을 만든다
	// 로비 UI로 이동한다
	
public:
	virtual void NativeConstruct() override;
	UPROPERTY()
	class UBanimalsGameInstance* gi;
	
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_DuoBlackHole;
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_TeamLuggage;

	// 버튼을 눌렀을때 호출할 함수
	UFUNCTION()
	void CreateBlackholeSession();
	UFUNCTION()
	void CreateLuggageSession();
};
