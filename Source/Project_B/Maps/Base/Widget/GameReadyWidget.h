// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameReadyWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_B_API UGameReadyWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(Transient,meta = (BindWidgetAnim))
	UWidgetAnimation* LoadComplete;

	void PlayAnimLoadComplete();

	virtual void OnAnimationFinished_Implementation(const UWidgetAnimation* Animation) override;
};
