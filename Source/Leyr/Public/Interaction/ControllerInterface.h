// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Inventory/InventoryComponent.h"
#include "ControllerInterface.generated.h"

enum class EContainerType : uint8;
struct FInventoryItemData;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UControllerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class LEYR_API IControllerInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void CloseInventory();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ToggleContainer(bool bOpen, int32 SlotCount);

	UFUNCTION(BlueprintNativeEvent)
	void UpdateInventorySlot(EContainerType ContainerType, int32 SlotIndex, FInventoryItemData ItemData);

	UFUNCTION(BlueprintNativeEvent)
	void ResetInventorySlot(EContainerType ContainerType, int32 SlotIndex);

	UFUNCTION(BlueprintNativeEvent)
	void UpdateContainerSlots(int32 TotalSlots);
};
