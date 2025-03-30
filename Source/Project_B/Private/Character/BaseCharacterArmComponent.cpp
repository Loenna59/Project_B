// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BaseCharacterArmComponent.h"

#include "Character/BaseCharacter.h"
#include "Character/BaseCharacterAnimInstance.h"
#include "Character/BaseCharacterPickComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Project_B/Utilities/TraceChannelHelper.h"
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
}


void UBaseCharacterArmComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                               FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

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
	
	AnimInstance->bPicking = true;
	
	TWeakObjectPtr<UBaseCharacterArmComponent> ThisWeak = this;
	
	FVector Location = Mesh->GetSocketLocation(SocketName);
	
	TraceChannelHelper::SphereMultiByChannel
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
		AnimInstance->IKTargetLocation = FVector::ZeroVector;
		AnimInstance->bPicking = false;
	}

	if (PhysicsHandleComp->GrabbedComponent)
	{
		PhysicsHandleComp->ReleaseComponent();
		GrabbedActor = nullptr;
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
			if (HitComp && HitComp->IsSimulatingPhysics())
			{
				FVector GrabLocation;
				float Distance = HitComp->GetClosestPointOnCollision(
					Mesh->GetSocketLocation(SocketName),
					GrabLocation
				);

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
					break;
				}
			}
		}
	}
}

