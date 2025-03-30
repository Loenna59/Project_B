// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SessionItemUI.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_B_API USessionItemUI : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_JoinSession;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_SessionName;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_EnterPlayer;


	int32 roomIdx;
	
	UFUNCTION()
	void Btn_JoinSession_Clicked();

	void SetInfo(int32 idx, FString info);
};
