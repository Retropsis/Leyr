// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "Inventory/InventoryComponent.h"
#include "UObject/Interface.h"
#include "PlayerInterface.generated.h"

enum class EArmorType : uint8;
enum class EContainerType : uint8;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPlayerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class LEYR_API IPlayerInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent)
	void UpdateInventorySlot(EContainerType ContainerType, int32 SlotIndex, FInventoryItemData ItemData);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnSlotDrop(EContainerType TargetContainer, EContainerType SourceContainer, int32 SourceSlotIndex, int32 TargetSlotIndex, EArmorType ArmorType);

	UFUNCTION(BlueprintNativeEvent)
	void ResetInventorySlot(EContainerType ContainerType, int32 SlotIndex);
};
