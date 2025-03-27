// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Animation/KickTraceChannelAnimNotifyState.h"

#include "Character/BaseCharacterAttackComponent.h"

void UKickTraceChannelAnimNotifyState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                                  float FrameDeltaTime)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);

	AActor* Owner = MeshComp->GetOwner();
	
	if (Owner)
	{
		UBaseCharacterAttackComponent* AttackComp = Cast<UBaseCharacterAttackComponent>(Owner->GetDefaultSubobjectByName(TEXT("AttackComp")));
	
		if (AttackComp)
		{
			AttackComp->OnKickTraceChannel();
		}
	}
}
