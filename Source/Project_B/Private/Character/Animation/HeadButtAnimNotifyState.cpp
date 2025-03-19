// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Animation/HeadButtAnimNotifyState.h"

#include "Character/BaseCharacterPhysicsAnimComponent.h"

void UHeadButtAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                           float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	AActor* Owner = MeshComp->GetOwner();
	
	if (Owner)
	{
		UBaseCharacterPhysicsAnimComponent* Head = Cast<UBaseCharacterPhysicsAnimComponent>(Owner->GetDefaultSubobjectByName(TEXT("HeadPhysicsAnimComp")));
	
		if (Head)
		{
			Head->TogglePhysicalAnimation(false);
		}
	}
}

void UHeadButtAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);

	AActor* Owner = MeshComp->GetOwner();
	
	if (Owner)
	{
		UBaseCharacterPhysicsAnimComponent* Head = Cast<UBaseCharacterPhysicsAnimComponent>(Owner->GetDefaultSubobjectByName(TEXT("HeadPhysicsAnimComp")));
	
		if (Head)
		{
			Head->TogglePhysicalAnimation(true);
		}
	}
}
