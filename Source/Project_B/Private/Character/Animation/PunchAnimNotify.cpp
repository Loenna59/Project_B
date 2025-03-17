// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Animation/PunchAnimNotify.h"

#include "GameFramework/Character.h"

void UPunchAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	AActor* Owner = MeshComp->GetOwner();
	
	if (Owner)
	{
		// if (ACharacter* Character = Cast<ACharacter>(Owner))
		// {
		// 	Character->LaunchCharacter(Character->GetActorForwardVector() * 100.f, false, false);
		// }
	}
}
