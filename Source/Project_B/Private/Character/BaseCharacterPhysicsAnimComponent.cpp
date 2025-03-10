#include "Character/BaseCharacterPhysicsAnimComponent.h"

#include "Character/BaseCharacter.h"
#include "PhysicsEngine/PhysicalAnimationComponent.h"
#include "Project_B/Utilities/LogMacro.h"


UBaseCharacterPhysicsAnimComponent::UBaseCharacterPhysicsAnimComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UBaseCharacterPhysicsAnimComponent::BeginPlay()
{
	Super::BeginPlay();

	UActorComponent* Comp = GetOwner()->AddComponentByClass(UPhysicalAnimationComponent::StaticClass(), true, FTransform::Identity, true);
	PhysicalAnimationComp = Cast<UPhysicalAnimationComponent>(Comp);

	Character = Cast<ABaseCharacter>(GetOwner());

	if (Character)
	{
		Mesh = Character->GetMesh();
		PhysicalAnimationComp->SetSkeletalMeshComponent(Mesh);
	}
	
}


void UBaseCharacterPhysicsAnimComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                                       FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::R))
	{
		TogglePhysicalAnimation();
	}

	if (Mesh && Mesh->IsSimulatingPhysics())
	{
		// 지정한 본의 up벡터 가져오기
		FVector CurrentUpVector = Mesh->GetBoneQuaternion(SimulateBoneName).Vector();

		// 회전을 보정하는 토크 적용 (외적)
		FVector Torque = FVector::CrossProduct(CurrentUpVector, FVector::UpVector) * 1000000.f;
		Mesh->AddTorqueInRadians(Torque, SimulateBoneName, true);
	}
}

void UBaseCharacterPhysicsAnimComponent::TogglePhysicalAnimation()
{
	// LOG_SCREEN("Toggle");
	Mesh->SetAllBodiesBelowSimulatePhysics(SimulateBoneName, true, false);
	// PhysicalAnimationComp->ApplyPhysicalAnimationProfileBelow(SimulateBoneName, TEXT("HitReactionProfile"), false, false);
	// PhysicalAnimationComp->SetStrengthMultiplyer(SimulateStrengthMultiplier);
	Mesh->SetAllBodiesBelowPhysicsBlendWeight(SimulateBoneName, .5f, false, true);
}

