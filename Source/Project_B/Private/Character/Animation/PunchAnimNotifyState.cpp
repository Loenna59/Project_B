#include "Character/Animation/PunchAnimNotifyState.h"

#include "Character/BaseCharacterPhysicsAnimComponent.h"

void UPunchAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                        float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	AActor* Owner = MeshComp->GetOwner();

	if (Owner)
	{
		UBaseCharacterPhysicsAnimComponent* Left = Cast<UBaseCharacterPhysicsAnimComponent>(Owner->GetDefaultSubobjectByName(TEXT("LeftArmPhysicsAnimComp")));

		if (Left)
		{
			Left->TogglePhysicalAnimation(false);
		}

		UBaseCharacterPhysicsAnimComponent* Right = Cast<UBaseCharacterPhysicsAnimComponent>(Owner->GetDefaultSubobjectByName(TEXT("RightArmPhysicsAnimComp")));

		if (Right)
		{
			Right->TogglePhysicalAnimation(false);
		}
	}
}

void UPunchAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
	
	AActor* Owner = MeshComp->GetOwner();

	if (Owner)
	{
		UBaseCharacterPhysicsAnimComponent* Left = Cast<UBaseCharacterPhysicsAnimComponent>(Owner->GetDefaultSubobjectByName(TEXT("LeftArmPhysicsAnimComp")));

		if (Left)
		{
			Left->TogglePhysicalAnimation(true);
		}

		UBaseCharacterPhysicsAnimComponent* Right = Cast<UBaseCharacterPhysicsAnimComponent>(Owner->GetDefaultSubobjectByName(TEXT("RightArmPhysicsAnimComp")));

		if (Right)
		{
			Right->TogglePhysicalAnimation(true);
		}
	}
}
