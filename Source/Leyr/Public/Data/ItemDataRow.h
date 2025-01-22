#pragma once

#include "CoreMinimal.h"
#include "Inventory/InventoryComponent.h"
#include "ItemDataRow.generated.h"

USTRUCT(Blueprintable, BlueprintType)
struct FItemDataRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FInventoryItemData ItemData = FInventoryItemData();
};