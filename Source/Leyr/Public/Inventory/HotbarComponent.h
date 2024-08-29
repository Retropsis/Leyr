// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "Inventory/InventoryComponent.h"
#include "HotbarComponent.generated.h"

/**
 * 
 */
UCLASS()
class LEYR_API UHotbarComponent : public UInventoryComponent
{
	GENERATED_BODY()

public:
	UHotbarComponent();
	
protected:
	virtual void HandleOnSlotDrop(UInventoryComponent* SourceInventory, int32 SourceSlotIndex, int32 TargetSlotIndex) override;
	virtual bool AddItemToIndex(FInventoryItemData InventoryItemData, int32 SourceSlotIndex, int32 TargetSlotIndex) override;
	virtual bool RemoveItemAtIndex(int32 SlotIndex) override;
};
