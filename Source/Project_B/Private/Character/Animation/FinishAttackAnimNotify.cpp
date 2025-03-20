// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Animation/FinishAttackAnimNotify.h"
#include "Character/BaseCharacterAttackComponent.h"

void UFinishAttackAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	AActor* Owner = MeshComp->GetOwner();
	
	if (Owner)
	{
		UBaseCharacterAttackComponent* AttackComp = Cast<UBaseCharacterAttackComponent>(Owner->GetDefaultSubobjectByName(TEXT("AttackComp")));
	
		if (AttackComp)
		{
			AttackComp->bIsAttacking = false;
		}
	}
}
