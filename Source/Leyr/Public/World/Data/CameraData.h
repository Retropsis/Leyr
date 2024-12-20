#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EExitDirection : uint8
{
	Vertical UMETA(DisplayName="Vertical"),
	Horizontal UMETA(DisplayName="Horizontal"),
};