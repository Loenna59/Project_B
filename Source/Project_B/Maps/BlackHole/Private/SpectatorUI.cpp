// Fill out your copyright notice in the Description page of Project Settings.


#include "Project_B/Maps/BlackHole/Public/SpectatorUI.h"

#include "Components/Button.h"
#include "Components/Image.h"

void USpectatorUI::NativeConstruct()
{
	Super::NativeConstruct();

	// 배열에 코인 이미지 추가
	CoinImages = { coin1, coin2, coin3, coin4, coin5 };
	
	InitializeCoinMaterials();
	
	coinCount = 3;
	btn_banana->OnClicked.AddDynamic(this, &USpectatorUI::Banana_Clicked);
	btn_poo->OnClicked.AddDynamic(this, &USpectatorUI::Poo_Clicked);
	btn_bomb->OnClicked.AddDynamic(this, &USpectatorUI::Bomb_Clicked);
}

void USpectatorUI::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	UpdateCoinProgress(InDeltaTime);
}

void USpectatorUI::Banana_Clicked()
{
	// 코인이 2개이상일때만 실행
	if (coinCount >= 2)
	{
		UE_LOG(LogTemp, Display, TEXT("Banana_Clicked"));
		// 관전자의 스폰 아이템은 바나나
		// 관전자 스폰 함수 실행
		coinCount -= 2;
	}
}

void USpectatorUI::Poo_Clicked()
{
	if (coinCount >= 3)
	{
		coinCount -= 3;
	}
}

void USpectatorUI::Bomb_Clicked()
{
	if (coinCount >= 4)
	{

		coinCount -= 4;
	}
}

void USpectatorUI::UpdateItemImages()
{
	
}

void USpectatorUI::UpdateCoinProgress(float DeltaTime)
{
	// 진행률 업데이트 (최대 5개까지)
	if (coinCount < 5)
	{
		float progressIncrement = DeltaTime / coinCreateTime;
		coinPercent += progressIncrement;
        
		// 퍼센트가 1.0이 되면 코인 카운트 증가시키자
		if (coinPercent >= 1.0f)
		{
			coinCount++;
			coinPercent = 0.0f;
		}
	}
    
	// 코인 업데이트
	for (int32 i = 0; i < CoinImages.Num(); i++)
	{
		if (CoinMaterials[i])
		{
			float percentToSet = 0.0f; // 기본 표시
            
			if (i < coinCount)
			{
				// 완전히 채워진 코인
				percentToSet = 1.0f;
			}
			else if (i == coinCount)
			{
				// 현재 채워지고 있는 코인
				// ... 이렇게 하는거래
				percentToSet = FMath::Max(0.0f, coinPercent);
			}
            
			// 머티리얼 파라미터 업데이트
			CoinMaterials[i]->SetScalarParameterValue(TEXT("Percent"), percentToSet);
		}
	}
}

void USpectatorUI::InitializeCoinMaterials()
{
	// 각 코인에 대한 동적 머티리얼 초기화
	CoinMaterials.SetNum(CoinImages.Num());
    
	for (int32 i = 0; i < CoinImages.Num(); i++)
	{
		if (CoinImages[i])
		{
			FSlateBrush brush = CoinImages[i]->GetBrush();
			if (brush.GetResourceObject() && brush.GetResourceObject()->IsA<UMaterialInterface>())
			{
				UMaterialInterface* baseMaterial = Cast<UMaterialInterface>(brush.GetResourceObject());
				CoinMaterials[i] = UMaterialInstanceDynamic::Create(baseMaterial, this);
                
				// 초기 퍼센트 값 설정
				CoinMaterials[i]->SetScalarParameterValue(TEXT("Percent"), 0.f);
                
				// 브러시를 동적 머티리얼로 업데이트
				brush.SetResourceObject(CoinMaterials[i]);
				CoinImages[i]->SetBrush(brush);
			}
		}
	}
}


