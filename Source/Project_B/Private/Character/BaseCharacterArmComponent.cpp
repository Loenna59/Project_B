// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BaseCharacterArmComponent.h"

#include "Character/BaseCharacter.h"
#include "Character/BaseCharacterAnimInstance.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Project_B/Utilities/LogMacro.h"
#include "Project_B/Utilities/TraceChannelHelper.h"


UBaseCharacterArmComponent::UBaseCharacterArmComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}


void UBaseCharacterArmComponent::BeginPlay()
{
	Super::BeginPlay();
}


void UBaseCharacterArmComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                               FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UBaseCharacterArmComponent::BeginGrab()
{
	if (!Character || !AnimInstance)
	{
		return;
	}

}

void UBaseCharacterArmComponent::Grabbing()
{
	if (!Character || !AnimInstance)
	{
		return;
	}

	if (bIsAttached)
	{
		return;
	}

	AnimInstance->bPicking = true;
	
	TWeakObjectPtr<UBaseCharacterArmComponent> ThisWeak = this;

	FVector Location = Mesh->GetSocketLocation(SocketName);
	
	TraceChannelHelper::SphereSingleByChannel
	(
		GetWorld(),
		Character,
		Location,
		Location,
		FRotator::ZeroRotator,
		ECC_Visibility,
		Radius,
		true,
		true,
		[ThisWeak] (bool bHit, FHitResult HitResult)
		{
			if (ThisWeak.IsValid())
			{
				ThisWeak->DetectNearby(bHit, HitResult);
			}
		}
	);
}

void UBaseCharacterArmComponent::ReleaseGrab()
{
	TogglePhysicalAnimation(true);

	if (AnimInstance)
	{
		AnimInstance->IKTargetLocation = FVector::ZeroVector;
		AnimInstance->bPicking = false;
	}

	bIsAttached = false;
	if (GrabConstraintComp)
	{
		GrabConstraintComp->BreakConstraint();
		GrabConstraintComp->DestroyComponent();
		GrabConstraintComp = nullptr;
	}
}

void UBaseCharacterArmComponent::DetectNearby(bool bHit, FHitResult HitResult)
{
	if (!Character)
	{
		return;
	}

	if (bHit)
	{
		AActor* Actor = HitResult.GetActor();
		TogglePhysicalAnimation(false);

		UPrimitiveComponent* Comp = HitResult.GetComponent();
		if (Comp)
		{
			// 손과 물체의 표면이 가까운지 거리 계산
			FVector HandClosestPoint;
			FVector OtherClosestPoint;

			Mesh->GetClosestPointOnCollision(HitResult.Location, HandClosestPoint);
			Comp->GetClosestPointOnCollision(HitResult.Location, OtherClosestPoint);

			float Distance = FVector::Distance(HandClosestPoint, OtherClosestPoint);

			if (Distance < AttachDistanceThreshold)
			{
				//LOG_SCREEN("%f", Distance);
				AttachTo(Comp, HitResult.Location, HitResult.Normal.Rotation());
			}
		}
		
		FVector TargetLocation = Actor->GetActorLocation();
			
		// AnimInstance->IKTargetLocation = FMath::VInterpTo(
		// 	AnimInstance->IKTargetLocation,
		// 	TargetLocation, GetWorld()->DeltaTimeSeconds,
		// 	2.5f
		// );

		// LOG_SCREEN("DetectNearby %s - %s", *HitResult.Location.ToString(), *HitResult.Normal.ToString());

		AnimInstance->IKTargetLocation = TargetLocation;
		return;
	}
	// LOG_SCREEN("DetectNearby None");
			
	AnimInstance->IKTargetLocation = FVector::ZeroVector;
	TogglePhysicalAnimation(true);
}

void UBaseCharacterArmComponent::AttachTo(UPrimitiveComponent* Comp, FVector Location, FRotator Rotation)
{
	bIsAttached = true;

	if (Comp && !GrabConstraintComp)
	{
		FTransform CompTransform = Comp->GetComponentTransform();
		
		GrabConstraintComp = NewObject<UPhysicsConstraintComponent>(this);
		GrabConstraintComp->RegisterComponent();
		GrabConstraintComp->AttachToComponent(Mesh, FAttachmentTransformRules::SnapToTargetIncludingScale, BoneName);

		Comp->SetWorldTransform(CompTransform, false, nullptr, ETeleportType::TeleportPhysics);
		
		GrabConstraintComp->SetConstrainedComponents(Mesh, BoneName, Comp, NAME_None);

		// 회전/이동을 제한하여 부드럽게 따라가도록 설정
		GrabConstraintComp->SetAngularSwing1Limit(ACM_Limited, 45.0f);
		GrabConstraintComp->SetAngularSwing2Limit(ACM_Limited, 45.0f);
		GrabConstraintComp->SetAngularTwistLimit(ACM_Limited, 45.f);
		GrabConstraintComp->SetLinearXLimit(LCM_Limited, 10.0f);
		GrabConstraintComp->SetLinearYLimit(LCM_Limited, 10.0f);
		GrabConstraintComp->SetLinearZLimit(LCM_Limited, 10.0f);
		
		// GrabComponent = Comp;
		// Comp->AttachToComponent(Mesh, FAttachmentTransformRules::SnapToTargetIncludingScale, SocketName);
	}
}

