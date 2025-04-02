#include "Character/BaseCharacterPhysicsAnimComponent.h"

#include "Character/BaseCharacter.h"
#include "Character/BaseCharacterAnimInstance.h"
#include "Net/UnrealNetwork.h"


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
		AnimInstance = Cast<UBaseCharacterAnimInstance>(Mesh->GetAnimInstance());
		PhysicalAnimationComp = Character->PhysicalAnimationComp;
	}

	TogglePhysicalAnimation(bAwakePhysics);
}

void UBaseCharacterPhysicsAnimComponent::GetLifetimeReplicatedProps(
	TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UBaseCharacterPhysicsAnimComponent, SimulateBoneName);
	DOREPLIFETIME(UBaseCharacterPhysicsAnimComponent, ReplicatedTorque);
}

void UBaseCharacterPhysicsAnimComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                                       FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!Character || !Character->HasAuthority())
	{
		return; // 서버에서만 실행
	}

	if (Mesh && Mesh->IsSimulatingPhysics())
	{
		// 지정한 본의 up벡터 가져오기
		FVector CurrentUpVector = Mesh->GetBoneQuaternion(SimulateBoneName).Vector();
	
		// 회전을 보정하는 토크 적용 (외적)
		ReplicatedTorque = FVector::CrossProduct(CurrentUpVector, FVector::UpVector) * 500000.f;
		
	}
}

void UBaseCharacterPhysicsAnimComponent::TogglePhysicalAnimation(bool toggle)
{
	if (!Character)
	{
		return;
	}

	if (!Character->IsLocallyControlled())
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
	if (!Character || Character->IsLocallyControlled() || Character->HasAuthority())
	{
		TogglePhysicalAnimationInternal(BoneName, bSimulate);
	}
}

void UBaseCharacterPhysicsAnimComponent::TogglePhysicalAnimationInternal(FName BoneName, bool bSimulate)
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

void UBaseCharacterPhysicsAnimComponent::OnRep_AddTorque()
{
	if (Character && !Character->HasAuthority())
	{
		Mesh->AddTorqueInRadians(ReplicatedTorque, SimulateBoneName, true);
	}
}



