// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BaseCharacterArmComponent.h"

#include "Character/BaseCharacter.h"
#include "Character/BaseCharacterAnimInstance.h"
#include "Character/BaseCharacterPickComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Project_B/Utilities/UTraceChannelHelper.h"
#include "Weapon/Weapon.h"

UBaseCharacterArmComponent::UBaseCharacterArmComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	// Physics Handle
	PhysicsHandleComp = CreateDefaultSubobject<UPhysicsHandleComponent>(TEXT("PhysicsHandle"));

}


void UBaseCharacterArmComponent::BeginPlay()
{
	Super::BeginPlay();

	if (!Character)
	{
		return;
	}
	
	PickComp = Cast<UBaseCharacterPickComponent>(Character->GetDefaultSubobjectByName(TEXT("PickComp")));
	PickComp->OnGrabbing.AddUObject(this, &UBaseCharacterArmComponent::Grabbing);
	PickComp->OnRelease.AddUObject(this, &UBaseCharacterArmComponent::ReleaseGrab);
	PickComp->OffPhysics.AddUObject(this, &UBaseCharacterArmComponent::CheckAndOffPhysics);
	OnUpdateGrabState.BindUObject(PickComp, &UBaseCharacterPickComponent::UpdateGrabState);

	PhysicsHandleComp->SetInterpolationSpeed(1000.f);
}


void UBaseCharacterArmComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                               FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (Character && Character->bHasWeapon)
	{
		return;
	}

	if (PhysicsHandleComp && PhysicsHandleComp->GrabbedComponent)
	{
		FVector TargetLocation = Mesh->GetSocketLocation(SocketName);
		PhysicsHandleComp->SetTargetLocation(TargetLocation);
	}
}

void UBaseCharacterArmComponent::Grabbing()
{
	if (!Character || !AnimInstance)
	{
		return;
	}
	
	if (Character->bHasWeapon)
	{
		return;
	}

	if (PickComp->BlockGrabState == GrabState)
	{
		return;
	}
	
	AnimInstance->bIsGrabbing[GrabState] = true;
	
	TWeakObjectPtr ThisWeak = this;
	
	FVector Location = Mesh->GetSocketLocation(SocketName);
	
	UTraceChannelHelper::SphereMultiByChannel
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
		[ThisWeak] (bool bHit, TArray<FHitResult> HitResults)
		{
			if (ThisWeak.IsValid())
			{
				ThisWeak->DetectNearby(bHit, HitResults);
			}
		}
	);
	
}

void UBaseCharacterArmComponent::ReleaseGrab()
{
	if (!Character->bHasWeapon)
	{
		TogglePhysicalAnimation(true);
	}
	
	if (AnimInstance)
	{
		AnimInstance->HandIKTarget[GrabState] = FVector::ZeroVector;
		AnimInstance->bIsGrabbing[GrabState] = false;
	}

	if (PhysicsHandleComp->GrabbedComponent)
	{
		PhysicsHandleComp->ReleaseComponent();
		GrabbedActor = nullptr;
		bool _ = Character->OnCalculateSpeedByMass.ExecuteIfBound(0);
		bool __ = OnUpdateGrabState.ExecuteIfBound(GrabState, false);
	}
}

void UBaseCharacterArmComponent::DetectNearby(bool bHit, TArray<FHitResult> HitResults)
{
	if (bHit)
	{
		if (Character->bHasWeapon)
		{
			return;
		}

		if (GrabbedActor != nullptr)
		{
			return;
		}
		
		for (FHitResult& Result : HitResults)
		{
			UPrimitiveComponent* HitComp = Result.GetComponent();

			if (HitComp == Mesh)
			{
				continue;
			}
			
			if (HitComp && HitComp->IsSimulatingPhysics())
			{
				FVector GrabLocation;
				float Distance = HitComp->GetClosestPointOnCollision(
					Mesh->GetSocketLocation(SocketName),
					GrabLocation
				);

				// 어깨 위치
				FVector ShoulderLocation = Mesh->GetSocketLocation(JointBoneName);
				FVector DirectionToHit = (Result.ImpactPoint - ShoulderLocation).GetSafeNormal();

				// 손 위치 보정 (최대 거리 제한)
				float MaxReach = GrabTraceDistance;
				FVector ClampedTarget = ShoulderLocation + DirectionToHit * FMath::Min(MaxReach, FVector::Dist(Result.ImpactPoint, ShoulderLocation));

				// 팔꿈치가 몸 바깥으로 꺾이도록 JointTarget 설정
				
				FVector ArmVector = GrabState == EGrabState::Left? -Character->GetActorRightVector() : Character->GetActorRightVector();

				FVector JointOffset = ArmVector * JointOffsetAmount + Character->GetActorForwardVector() * 10.f;  // 오른손 기준 오른쪽
				FVector JointTarget = ClampedTarget + JointOffset;

				if (Distance >= 0.f)
				{
					AActor* HitActor = HitComp->GetOwner();
					if (HitActor->IsA<AWeapon>() && HitActor->GetOwner() == nullptr)
					{
						Character->TakeWeapon(Cast<AWeapon>(HitActor));
						break;
					}
					
					PhysicsHandleComp->GrabComponentAtLocation(HitComp, NAME_None, GrabLocation);
					GrabbedActor = HitActor;

					bool _ = Character->OnCalculateSpeedByMass.ExecuteIfBound(HitComp->GetMass());
					bool __ = OnUpdateGrabState.ExecuteIfBound(GrabState, true);

					if (AnimInstance)
					{
						AnimInstance->bIsGrabbing[GrabState] = true;
						AnimInstance->HandIKTarget[GrabState] = GrabLocation;
						AnimInstance->HandIKJoint[GrabState] = JointTarget;
					}

					// 붙으면 physics 끄쟈
					TogglePhysicalAnimation(false);
					break;
				}
			}
		}
	}
}

void UBaseCharacterArmComponent::CheckAndOffPhysics(EGrabState BlockState)
{
	if (GrabState == BlockState)
	{
		AnimInstance->bIsGrabbing[GrabState] = false;
		TogglePhysicalAnimation(false);
	}
}

