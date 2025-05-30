#pragma once

#include "CoreMinimal.h"
#include "InventoryData.generated.h"

UENUM(BlueprintType)
enum class EContainerSubType : uint8
{
	Chest UMETA(DisplayName="Chest"),
	PlayerBag UMETA(DisplayName="PlayerBag"),
	Trader UMETA(DisplayName="Trader"),
	CraftingTable UMETA(DisplayName="CraftingTable"),
	Bonfire UMETA(DisplayName="Bonfire")
};
