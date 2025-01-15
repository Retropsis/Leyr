// @ Retropsis 2024-2025.

#include "Inventory/PlayerInventoryComponent.h"
#include "Interaction/PlayerInterface.h"

UPlayerInventoryComponent::UPlayerInventoryComponent()
{
	Items.SetNum(30);
}

void UPlayerInventoryComponent::HandleOnSlotDrop(UInventoryComponent* SourceInventory, int32 SourceSlotIndex,  int32 TargetSlotIndex)
{
	if(SourceInventory == this && SourceSlotIndex == TargetSlotIndex) return;

	switch (SourceInventory->ContainerType)
	{
	case EContainerType::Inventory:
	case EContainerType::Hotbar:
	case EContainerType::Container:
		SourceInventory->TransferItem(this, SourceSlotIndex, TargetSlotIndex);
		break;
	case EContainerType::Equipment:
		break;
	default: ;
	}
}

bool UPlayerInventoryComponent::AddItemToIndex(FInventoryItemData InventoryItemData, int32 SourceSlotIndex, int32 TargetSlotIndex)
{
	const bool bWasSuccessful = Super::AddItemToIndex(InventoryItemData, SourceSlotIndex, TargetSlotIndex);

	IPlayerInterface::Execute_UpdateInventorySlot(GetOwner(), ContainerType, TargetSlotIndex, GetItemAtIndex(TargetSlotIndex));
	
	return bWasSuccessful;
}

bool UPlayerInventoryComponent::RemoveItemAtIndex(int32 SlotIndex)
{
	const bool bWasSuccessful = Super::RemoveItemAtIndex(SlotIndex);

	IPlayerInterface::Execute_ResetInventorySlot(GetOwner(), ContainerType, SlotIndex);

	return bWasSuccessful;
}

/*
 * Custom Functions
 */
bool UPlayerInventoryComponent::UpdateItemInputTag(FInventoryItemData ItemData, FGameplayTag InputTag)
{
	for (int i = 0; i < Items.Num(); ++i)
	{
		if(Items[i].ID == ItemData.ID)
		{
			Items[i].InputTag = InputTag;
			return true;
		}
	}
	return false;
}