#pragma once

#include "CoreMinimal.h"
#include "LevelDesignData.generated.h"

UENUM(BlueprintType)
enum class EBuildPattern : uint8
{
	None,
	Horizontal,
	Vertical,
	Diagonal_Up,
	Diagonal_Down,
};