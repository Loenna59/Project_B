// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "BlackholePlayerState.generated.h"

DECLARE_DELEGATE_OneParam(FUpdateName, FString);

UCLASS()
class PROJECT_B_API ABlackholePlayerState : public APlayerState
{
	GENERATED_BODY()
public:
	//virtual void OnRep_UniqueId() override;

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	// 관전자 모드 전환 RPC
};
