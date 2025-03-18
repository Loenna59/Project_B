// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Animation/KickAnimNotify.h"

#include "Character/BaseCharacterPhysicsAnimComponent.h"
#include "GameFramework/Character.h"

void UKickAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	
	AActor* Owner = MeshComp->GetOwner();
	
	if (Owner)
	{
		if (ACharacter* Character = Cast<ACharacter>(Owner))
		{
			Character->LaunchCharacter(Character->GetActorForwardVector() * 1500.f, false, false);
		}
	}
}
