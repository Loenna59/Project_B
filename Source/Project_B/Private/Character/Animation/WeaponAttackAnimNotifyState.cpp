// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Animation/WeaponAttackAnimNotifyState.h"

#include "Character/BaseCharacter.h"

void UWeaponAttackAnimNotifyState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                              float FrameDeltaTime)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);

	AActor* Owner = MeshComp->GetOwner();
	
	if (Owner)
	{
		if (ABaseCharacter* Character = Cast<ABaseCharacter>(Owner))
		{
			Character->OnWeaponAttackTraceChannel();
		}
	}
}
