#pragma once

#include "CoreMinimal.h"
#include "UTraceChannelHelper.generated.h"

// Callback으로 사용할 Delegate 정의
DECLARE_DYNAMIC_DELEGATE_TwoParams(FOnSingleTraceCompleted, bool, bHit, const FHitResult&, HitResult);
DECLARE_DYNAMIC_DELEGATE_TwoParams(FOnMultiTraceCompleted, bool, bHit, const TArray<FHitResult>&, HitResult);

UCLASS()
class PROJECT_B_API UTraceChannelHelper : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UTraceChannelHelper();

	static void LineSingleByChannel(
		const UWorld* World,
		AActor* Actor,
		const FVector& Start,
		const FVector& End,
		ECollisionChannel CollisionChannel,
		bool IgnoreSelf,
		bool DrawDebug,
		TFunction<void(bool, FHitResult)> Callback
	);

	static void SphereSingleByChannel(
		const UWorld* World,
		AActor* Actor,
		const FVector& Start,
		const FVector& End,
		const FRotator& Rotator,
		ECollisionChannel CollisionChannel,
		float Radius,
		bool IgnoreSelf,
		bool DrawDebug,
		TFunction<void(bool, struct FHitResult)> Callback
	);

	static void BoxSingleByChannel(
		const UWorld* World,
		AActor* Actor,
		const FVector& Start,
		const FVector& End,
		const FRotator& Rotator,
		ECollisionChannel CollisionChannel,
		const FVector& HalfSize,
		bool IgnoreSelf,
		bool DrawDebug,
		TFunction<void(bool, FHitResult)> Callback
	);

	static void LineMultiByChannel(
		const UWorld* World,
		AActor* Actor,
		const FVector& Start,
		const FVector& End,
		ECollisionChannel CollisionChannel,
		bool IgnoreSelf,
		bool DrawDebug,
		TFunction<void(bool, TArray<struct FHitResult>)> Callback
	);

	static void SphereMultiByChannel(
		const UWorld* World,
		AActor* Actor,
		const FVector& Start,
		const FVector& End,
		const FRotator& Rotator,
		ECollisionChannel CollisionChannel,
		float Radius,
		bool IgnoreSelf,
		bool DrawDebug,
		TFunction<void(bool, TArray<struct FHitResult>)> Callback
	);
	
	static void BoxMultiByChannel(
		const UWorld* World,
		AActor* Actor,
		const FVector& Start,
		const FVector& End,
		const FRotator& Rotator,
		ECollisionChannel CollisionChannel,
		const FVector& HalfSize,
		bool IgnoreSelf,
		bool DrawDebug,
		TFunction<void(bool, TArray<struct FHitResult>)> Callback
	);

	static void SphereMultiByChannel(
		const UWorld* World,
		const FVector& Start,
		const FVector& End,
		const FRotator& Rotator,
		ECollisionChannel CollisionChannel,
		float Radius,
		TArray<AActor*> IgnoreActors,
		bool DrawDebug,
		TFunction<void(bool, TArray<struct FHitResult>)> Callback
	);
};
