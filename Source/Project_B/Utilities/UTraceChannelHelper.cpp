#include "UTraceChannelHelper.h"

#include "KismetTraceUtils.h"

UTraceChannelHelper::UTraceChannelHelper()
{
}

void UTraceChannelHelper::LineSingleByChannel(
	const UWorld* World,
	AActor* Actor,
	const FVector& Start,
	const FVector& End,
	ECollisionChannel CollisionChannel,
	bool IgnoreSelf,
	bool DrawDebug,
	TFunction<void(bool, FHitResult)> Callback
)
{
	FHitResult HitResult;
	FCollisionQueryParams CollisionParams;

	if (IgnoreSelf)
	{
		CollisionParams.AddIgnoredActor(Actor);
	}

	bool bHit = World->LineTraceSingleByChannel(
		HitResult,
		Start,
		End,
		CollisionChannel,
		CollisionParams
	);

	if (DrawDebug)
	{
		DrawDebugLineTraceSingle(
			World,
			Start,
			End,
			EDrawDebugTrace::ForDuration,
			bHit,
			HitResult,
			FColor::Yellow,
			FColor::Green,
			1.f
		);
	}

	Callback(bHit, HitResult);
}

void UTraceChannelHelper::SphereSingleByChannel(
	const UWorld* World,
	AActor* Actor,
	const FVector& Start,
	const FVector& End,
	const FRotator& Rotator,
	ECollisionChannel CollisionChannel,
	float Radius,
	bool IgnoreSelf,
	bool DrawDebug,
	TFunction<void(bool, FHitResult)> Callback
)
{
	FHitResult HitResult;
	FCollisionQueryParams CollisionParams;

	if (IgnoreSelf)
	{
		CollisionParams.AddIgnoredActor(Actor);
	}
	
	bool bHit = World->SweepSingleByChannel(
		HitResult,
		Start,
		End,
		Rotator.Quaternion(),
		CollisionChannel,
		FCollisionShape::MakeSphere(Radius),
		CollisionParams
	);

	if (DrawDebug)
	{
		DrawDebugSphereTraceSingle(
			World,
			Start,
			End,
			Radius,
			EDrawDebugTrace::ForDuration,
			bHit,
			HitResult,
			FColor::Yellow,
			FColor::Green,
			1.f
		);
	}

	Callback(bHit, HitResult);
}

void UTraceChannelHelper::BoxSingleByChannel(
	const UWorld* World,
	AActor* Actor,
	const FVector& Start,
	const FVector& End,
	const FRotator& Rotator,
	ECollisionChannel CollisionChannel,
	const FVector& HalfSize,
	bool IgnoreSelf,
	bool DrawDebug,
	TFunction<void(bool, FHitResult)> Callback)
{
	FHitResult HitResult;
	FCollisionQueryParams CollisionParams;

	if (IgnoreSelf)
	{
		CollisionParams.AddIgnoredActor(Actor);
	}
	
	bool bHit = World->SweepSingleByChannel(
		HitResult,
		Start,
		End,
		Rotator.Quaternion(),
		CollisionChannel,
		FCollisionShape::MakeBox(HalfSize),
		CollisionParams
	);

	if (DrawDebug)
	{
		DrawDebugBoxTraceSingle(
			World,
			Start,
			End,
			HalfSize,
			Rotator,
			EDrawDebugTrace::ForDuration,
			bHit,
			HitResult,
			FColor::Yellow,
			FColor::Green,
			1.f
		);
	}

	Callback(bHit, HitResult);
}

void UTraceChannelHelper::LineMultiByChannel(
	const UWorld* World,
	AActor* Actor,
	const FVector& Start,
	const FVector& End,
	ECollisionChannel CollisionChannel,
	bool IgnoreSelf,
	bool DrawDebug,
	TFunction<void(bool, TArray<struct FHitResult>)> Callback
)
{
	TArray<FHitResult> HitResults;
	
	FCollisionQueryParams CollisionParams;

	if (IgnoreSelf)
	{
		CollisionParams.AddIgnoredActor(Actor);
	}

	bool bHit = World->LineTraceMultiByChannel(
		HitResults,
		Start,
		End,
		CollisionChannel,
		CollisionParams
	);

	if (DrawDebug)
	{
		DrawDebugLineTraceMulti(
			World,
			Start,
			End,
			EDrawDebugTrace::ForDuration,
			bHit,
			HitResults,
			FColor::Yellow,
			FColor::Green,
			1.f
		);
	}

	Callback(bHit, HitResults);
}

void UTraceChannelHelper::BoxMultiByChannel(
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
)
{
	TArray<FHitResult> HitResults;
	
	FCollisionQueryParams Params;
	if (IgnoreSelf)
	{
		Params.AddIgnoredActor(Actor);
	}
	
	bool bHit = World->SweepMultiByChannel(
		HitResults,
		Start,
		End,
		Rotator.Quaternion(),
		CollisionChannel, // "Player"
		FCollisionShape::MakeBox(HalfSize),
		Params
	);

	if (DrawDebug)
	{
		DrawDebugBoxTraceMulti(
			World,
			Start,
			End,
			HalfSize,
			Rotator,
			EDrawDebugTrace::ForDuration,
			bHit,
			HitResults,
			FColor::Yellow,
			FColor::Green,
			1.f
		);
	}

	Callback(bHit, HitResults);
}

void UTraceChannelHelper::SphereMultiByChannel(
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
)
{
	TArray<FHitResult> HitResults;
	
	FCollisionQueryParams Params;
	if (IgnoreSelf)
	{
		Params.AddIgnoredActor(Actor);
	}
	
	bool bHit = World->SweepMultiByChannel(
		HitResults,
		Start,
		End,
		Rotator.Quaternion(),
		CollisionChannel, // "Player"
		FCollisionShape::MakeSphere(Radius),
		Params
	);

	if (DrawDebug)
	{
		DrawDebugSphereTraceMulti(
			World,
			Start,
			End,
			Radius,
			EDrawDebugTrace::ForOneFrame,
			bHit,
			HitResults,
			FColor::Yellow,
			FColor::Green,
			1.f
		);
	}

	Callback(bHit, HitResults);
}

void UTraceChannelHelper::SphereMultiByChannel(
	const UWorld* World,
	const FVector& Start,
	const FVector& End,
	const FRotator& Rotator,
	ECollisionChannel CollisionChannel,
	float Radius,
	TArray<AActor*> IgnoreActors,
	bool DrawDebug,
	TFunction<void(bool, TArray<struct FHitResult>)> Callback
)
{
	TArray<FHitResult> HitResults;
	
	FCollisionQueryParams Params;

	if (!IgnoreActors.IsEmpty())
	{
		for (AActor* Actor : IgnoreActors)
		{
			Params.AddIgnoredActor(Actor);
		}
	}
	
	bool bHit = World->SweepMultiByChannel(
		HitResults,
		Start,
		End,
		Rotator.Quaternion(),
		CollisionChannel, 
		FCollisionShape::MakeSphere(Radius),
		Params
	);

	if (DrawDebug)
	{
		DrawDebugSphereTraceMulti(
			World,
			Start,
			End,
			Radius,
			EDrawDebugTrace::ForOneFrame,
			bHit,
			HitResults,
			FColor::Yellow,
			FColor::Green,
			1.f
		);
	}

	Callback(bHit, HitResults);
}