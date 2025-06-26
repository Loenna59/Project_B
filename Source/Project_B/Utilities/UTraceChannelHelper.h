#pragma once

#include "CoreMinimal.h"
#include "UTraceChannelHelper.generated.h"

// Callback으로 사용할 Delegate 정의
DECLARE_DELEGATE_TwoParams(FOnSingleTraceCompleted, bool, const FHitResult&);
DECLARE_DELEGATE_TwoParams(FOnMultiTraceCompleted, bool, const TArray<FHitResult>&);

UCLASS()
class PROJECT_B_API UTraceChannelHelper : public UObject
{
	GENERATED_BODY()
	
public:
	UTraceChannelHelper();
	
	static void LineSingleByChannel(
		UObject* WorldContextObject,
		AActor* Actor,
		const FVector& Start,
		const FVector& End,
		ECollisionChannel CollisionChannel,
		bool IgnoreSelf,
		bool DrawDebug,
		FOnSingleTraceCompleted OnCompleted
	);
	
	static void SphereSingleByChannel(
		UObject* WorldContextObject,
		AActor* Actor,
		const FVector& Start,
		const FVector& End,
		const FRotator& Rotator,
		ECollisionChannel CollisionChannel,
		float Radius,
		bool IgnoreSelf,
		bool DrawDebug,
		FOnSingleTraceCompleted OnCompleted
	);
	
	static void BoxSingleByChannel(
		UObject* WorldContextObject,
		AActor* Actor,
		const FVector& Start,
		const FVector& End,
		const FRotator& Rotator,
		ECollisionChannel CollisionChannel,
		const FVector& HalfSize,
		bool IgnoreSelf,
		bool DrawDebug,
		FOnSingleTraceCompleted OnCompleted
	);
	
	static void LineMultiByChannel(
		UObject* WorldContextObject,
		AActor* Actor,
		const FVector& Start,
		const FVector& End,
		ECollisionChannel CollisionChannel,
		bool IgnoreSelf,
		bool DrawDebug,
		FOnMultiTraceCompleted OnCompleted
	);
	
	static void SphereMultiByChannel(
		UObject* WorldContextObject,
		AActor* Actor,
		const FVector& Start,
		const FVector& End,
		const FRotator& Rotator,
		ECollisionChannel CollisionChannel,
		float Radius,
		bool IgnoreSelf,
		bool DrawDebug,
		FOnMultiTraceCompleted OnCompleted
	);
	
	static void BoxMultiByChannel(
		UObject* WorldContextObject,
		AActor* Actor,
		const FVector& Start,
		const FVector& End,
		const FRotator& Rotator,
		ECollisionChannel CollisionChannel,
		const FVector& HalfSize,
		bool IgnoreSelf,
		bool DrawDebug,
		FOnMultiTraceCompleted OnCompleted
	);

	static void SphereMultiByChannel(
		UObject* WorldContextObject,
		const FVector& Start,
		const FVector& End,
		const FRotator& Rotator,
		ECollisionChannel CollisionChannel,
		float Radius,
		TArray<AActor*> IgnoreActors,
		bool DrawDebug,
		FOnMultiTraceCompleted OnCompleted
	);
};
