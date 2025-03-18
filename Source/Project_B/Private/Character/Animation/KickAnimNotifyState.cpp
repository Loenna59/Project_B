#include "Character/Animation/KickAnimNotifyState.h"

#include "Character/BaseCharacterPhysicsAnimComponent.h"

void UKickAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                       float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	AActor* Owner = MeshComp->GetOwner();
	
	if (Owner)
	{
		UBaseCharacterPhysicsAnimComponent* RightFoot = Cast<UBaseCharacterPhysicsAnimComponent>(Owner->GetDefaultSubobjectByName(TEXT("RightFootPhysicsAnimComp")));

		if (RightFoot)
		{
			RightFoot->TogglePhysicalAnimation(true);
			// RightFoot->AddForceForwardVector();
		}
	}
}

void UKickAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);

	AActor* Owner = MeshComp->GetOwner();
	
	if (Owner)
	{
		UBaseCharacterPhysicsAnimComponent* RightFoot = Cast<UBaseCharacterPhysicsAnimComponent>(Owner->GetDefaultSubobjectByName(TEXT("RightFootPhysicsAnimComp")));

		if (RightFoot)
		{
			RightFoot->TogglePhysicalAnimation(false);
		}
	}
}
