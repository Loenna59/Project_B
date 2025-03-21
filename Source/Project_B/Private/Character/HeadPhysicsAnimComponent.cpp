
#include "Character/HeadPhysicsAnimComponent.h"
#include "Character/BaseCharacter.h"


UHeadPhysicsAnimComponent::UHeadPhysicsAnimComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
}

void UHeadPhysicsAnimComponent::BeginPlay()
{
	Super::BeginPlay();

	TogglePhysicalAnimation(true);
}

void UHeadPhysicsAnimComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                              FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (Mesh && Mesh->IsSimulatingPhysics())
	{
		FVector CurrentVelocity = Character->GetVelocity();
		FVector VelocityChange = (CurrentVelocity - PreviousVelocity) / DeltaTime;
		PreviousVelocity = CurrentVelocity;
	
		if (VelocityChange.Size() > Threshold) // Threshold는 임계값
		{
			FVector ForceDirection = -VelocityChange * 1000.0f; // 반대 방향으로 힘 적용
			Mesh->AddImpulse(ForceDirection, SimulateBoneName);
		}
	}
}

