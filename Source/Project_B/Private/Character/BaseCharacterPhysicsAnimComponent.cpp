#include "Character/BaseCharacterPhysicsAnimComponent.h"

#include "Character/BaseCharacter.h"
#include "Net/UnrealNetwork.h"
#include "PhysicsEngine/PhysicalAnimationComponent.h"
#include "Project_B/Utilities/LogMacro.h"


UBaseCharacterPhysicsAnimComponent::UBaseCharacterPhysicsAnimComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	
	SetIsReplicatedByDefault(true);
}


void UBaseCharacterPhysicsAnimComponent::BeginPlay()
{
	Super::BeginPlay();

	Character = Cast<ABaseCharacter>(GetOwner());

	if (Character)
	{
		Mesh = Character->GetMesh();
		PhysicalAnimationComp = Character->PhysicalAnimationComp;
	}

	TogglePhysicalAnimation(bAwakePhysics);
}

void UBaseCharacterPhysicsAnimComponent::GetLifetimeReplicatedProps(
	TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UBaseCharacterPhysicsAnimComponent, SimulateBoneName);
}

void UBaseCharacterPhysicsAnimComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                                       FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (Mesh && Mesh->IsSimulatingPhysics())
	{
		// 지정한 본의 up벡터 가져오기
		FVector CurrentUpVector = Mesh->GetBoneQuaternion(SimulateBoneName).Vector();
	
		// 회전을 보정하는 토크 적용 (외적)
		FVector Torque = FVector::CrossProduct(CurrentUpVector, FVector::UpVector) * 500000.f;
		Mesh->AddTorqueInRadians(Torque, SimulateBoneName, true);
	}
}

void UBaseCharacterPhysicsAnimComponent::TogglePhysicalAnimation(bool toggle)
{
	if (!Character)
	{
		return;
	}
	
	Server_TogglePhysicalAnimation(SimulateBoneName, toggle);
}

void UBaseCharacterPhysicsAnimComponent::Server_TogglePhysicalAnimation_Implementation(FName BoneName, bool bSimulate)
{
	Multicast_TogglePhysicalAnimation(BoneName, bSimulate);
}

void UBaseCharacterPhysicsAnimComponent::Multicast_TogglePhysicalAnimation_Implementation(FName BoneName,
	bool bSimulate)
{
	if (!Mesh)
	{
		return;
	}
	
	if (bSimulate)
	{
		Mesh->SetAllBodiesBelowSimulatePhysics(BoneName, true, false);
		Mesh->SetAllBodiesBelowPhysicsBlendWeight(BoneName, 0.5f, false, true);
		return;
	}
	
	Mesh->SetAllBodiesBelowSimulatePhysics(BoneName, false, false);
}

void UBaseCharacterPhysicsAnimComponent::AddForceForwardVector()
{
	if (Character)
	{
		FVector ForceDirection = Character->GetActorForwardVector() * ForwardForceAmount; // 앞방향으로 500 단위의 힘
		Mesh->AddImpulseToAllBodiesBelow(ForceDirection, SimulateBoneName, false);
	}
}



