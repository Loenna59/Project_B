#pragma once

#include "CoreMinimal.h"
#include "AttackType.generated.h"

UENUM(BlueprintType)
enum class EAttackType : uint8
{
	PUNCH,
	HEAD_BUTT,
	KICK,
	HAMMER,
	BOTTLE,
	CROSS_BOW
};