// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CreateLobbyUI.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_B_API UCreateLobbyUI : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	UPROPERTY()
	class UBanimalsGameInstance* gi;
	
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_JoinSession;

	// 버튼을 눌렀을때 호출할 함수
	UFUNCTION()
	void JoinSession();
};
