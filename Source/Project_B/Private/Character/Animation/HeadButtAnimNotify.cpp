// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Animation/HeadButtAnimNotify.h"

#include "Character/BaseCharacterPhysicsAnimComponent.h"

void UHeadButtAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	AActor* Owner = MeshComp->GetOwner();
	
	if (Owner)
	{
		UBaseCharacterPhysicsAnimComponent* Comp = Cast<UBaseCharacterPhysicsAnimComponent>(Owner->GetDefaultSubobjectByName(TEXT("HeadPhysicsAnimComp")));
	
		if (Comp)
		{
			Comp->AddForceForwardVector();
		}
		
	}
}
