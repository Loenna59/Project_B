#include "UTraceChannelHelper.h"

#include "KismetTraceUtils.h"
#include "Kismet/KismetSystemLibrary.h"

UTraceChannelHelper::UTraceChannelHelper()
{
}

void UTraceChannelHelper::LineSingleByChannel
(
	UObject* WorldContextObject,
	AActor* Actor,
	const FVector& Start,
	const FVector& End,
	ECollisionChannel CollisionChannel,
	bool IgnoreSelf,
	bool DrawDebug,
	FOnSingleTraceCompleted OnCompleted
)
{
	const UWorld* World = GEngine->GetWorldFromContextObject
	(
		WorldContextObject, EGetWorldErrorMode::LogAndReturnNull
	);

	if (!World)
	{
		return;
	}
	
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

	if (OnCompleted.IsBound())
	{
		OnCompleted.Execute(bHit, HitResult);
	}
}

void UTraceChannelHelper::SphereSingleByChannel
(
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
)
{
	const UWorld* World = GEngine->GetWorldFromContextObject
	(
		WorldContextObject, EGetWorldErrorMode::LogAndReturnNull
	);

	if (!World)
	{
		return;
	}
	
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

	if (OnCompleted.IsBound())
	{
		OnCompleted.Execute(bHit, HitResult);
	}
}

void UTraceChannelHelper::BoxSingleByChannel
(
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
)
{
	const UWorld* World = GEngine->GetWorldFromContextObject
	(
		WorldContextObject, EGetWorldErrorMode::LogAndReturnNull
	);

	if (!World)
	{
		return;
	}

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

	if (OnCompleted.IsBound())
	{
		OnCompleted.Execute(bHit, HitResult);
	}
}

void UTraceChannelHelper::LineMultiByChannel
(
	UObject* WorldContextObject,
	AActor* Actor,
	const FVector& Start,
	const FVector& End,
	ECollisionChannel CollisionChannel,
	bool IgnoreSelf,
	bool DrawDebug,
	FOnMultiTraceCompleted OnCompleted
)
{
	const UWorld* World = GEngine->GetWorldFromContextObject
	(
		WorldContextObject, EGetWorldErrorMode::LogAndReturnNull
	);

	if (!World)
	{
		return;
	}

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

	if (OnCompleted.IsBound())
	{
		OnCompleted.Execute(bHit, HitResults);
	}
}

void UTraceChannelHelper::SphereMultiByChannel
(
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
)
{
	const UWorld* World = GEngine->GetWorldFromContextObject
	(
		WorldContextObject, EGetWorldErrorMode::LogAndReturnNull
	);

	if (!World)
	{
		return;
	}

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

	if (OnCompleted.IsBound())
	{
		OnCompleted.Execute(bHit, HitResults);
	}
}

void UTraceChannelHelper::BoxMultiByChannel(
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
)
{
	const UWorld* World = GEngine->GetWorldFromContextObject
	(
		WorldContextObject, EGetWorldErrorMode::LogAndReturnNull
	);

	if (!World)
	{
		return;
	}
	
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

	if (OnCompleted.IsBound())
	{
		OnCompleted.Execute(bHit, HitResults);
	}
}

void UTraceChannelHelper::SphereMultiByChannel(
	UObject* WorldContextObject,
	const FVector& Start,
	const FVector& End,
	const FRotator& Rotator,
	ECollisionChannel CollisionChannel,
	float Radius,
	TArray<AActor*> IgnoreActors,
	bool DrawDebug,
	FOnMultiTraceCompleted OnCompleted
)
{
	const UWorld* World = GEngine->GetWorldFromContextObject
	(
		WorldContextObject, EGetWorldErrorMode::LogAndReturnNull
	);

	if (!World)
	{
		return;
	}
	
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

	if (OnCompleted.IsBound())
	{
		OnCompleted.Execute(bHit, HitResults);
	}
}
