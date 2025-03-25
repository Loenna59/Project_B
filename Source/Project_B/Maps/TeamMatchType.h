#pragma once

#include "TeamMatchType.generated.h"

UENUM(BlueprintType)
enum class ETeamType : uint8
{
	None,
	Red,
	Blue,
	Yellow,
	Green
};