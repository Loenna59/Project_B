// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Animation/PunchAnimNotify.h"

#include "Character/BaseCharacterAttackComponent.h"

void UPunchAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	AActor* Owner = MeshComp->GetOwner();
	
	if (Owner)
	{
		UBaseCharacterAttackComponent* AttackComponent = Cast<UBaseCharacterAttackComponent>(Owner->GetDefaultSubobjectByName(TEXT("AttackComp")));
	
		if (AttackComponent)
		{
			AttackComponent->AddForceForwardVector();
		}
		
	}
}
