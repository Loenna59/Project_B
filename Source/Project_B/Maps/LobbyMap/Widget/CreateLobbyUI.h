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

	// 위젯 스위처
	// 0: 기본 시작, 1: 커스텀 게임
	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* WidgetSwitcher;
	
	// 0: 맨처음에 들어왔을 때 보일 화면
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_MainCustomGame;
	// 누르면 이동
	UFUNCTION()
	void Btn_MainCustomGame_Clicked();

	// 1: 커스텀 게임을 누르면 보일 화면
	// 새로고침하면 -> 세션 목록을 볼 수 있고, 방 생성하기
	UPROPERTY(meta = (BindWidget))
	class UScrollBox* ScrollBox;
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_FindSession; // 세션찾기 (새로고침)
	UPROPERTY(editanywhere)
	TSubclassOf<class USessionItemUI> sessionItemFactory;
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_GoFirst; // 뒤로가기
	
	// 버튼을 눌렀을때 호출할 함수
	UFUNCTION()
	void Btn_FindSession_Clicked();
	UFUNCTION()
	void Btn_GoFirst_Clicked();

	// 델리게이트 함수
	void OnFindComplete(int32 idx,FString info);
};
