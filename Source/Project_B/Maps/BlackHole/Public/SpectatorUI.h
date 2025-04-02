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
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
	// 아이템 버튼
	UPROPERTY(meta = (BindWidget))
	class UButton* btn_banana;
	UPROPERTY(meta = (BindWidget))
	class UButton* btn_poo;
	UPROPERTY(meta = (BindWidget))
	class UButton* btn_bomb;

	// 클릭시 실행될 함수
	UFUNCTION()
	void Banana_Clicked();
	UFUNCTION()
	void Poo_Clicked();
	UFUNCTION()
	void Bomb_Clicked();
	
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

	// 코인 로직
	int32 coinCount = 0; // 사실 시작시 3, 이러면 노란 코인이 3개 생성 (최대5)
	float coinCreateTime = 10; // 코인은 10초마다 하나씩 생성
	float coinPercent = 0;
	void UpdateItemImages();
	void UpdateCoinProgress(float DeltaTime);

	// 코인 머티리얼 초기화 함수
	void InitializeCoinMaterials();
	
	UPROPERTY()
	TArray<UImage*> CoinImages;
	UPROPERTY()
	TArray<UMaterialInstanceDynamic*> CoinMaterials;
};


