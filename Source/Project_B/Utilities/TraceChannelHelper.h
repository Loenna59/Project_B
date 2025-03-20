#pragma once

#include "CoreMinimal.h"

class PROJECT_B_API TraceChannelHelper
{
public:
	TraceChannelHelper();
	~TraceChannelHelper();

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
};
