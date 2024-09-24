// @ Retropsis 2024-2025.

#include "Inventory/HotbarComponent.h"

#include "Interaction/PlayerInterface.h"

UHotbarComponent::UHotbarComponent()
{
	Items.SetNum(4);
}

void UHotbarComponent::HandleOnSlotDrop(UInventoryComponent* SourceInventory, int32 SourceSlotIndex, int32 TargetSlotIndex)
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

bool UHotbarComponent::AddItemToIndex(FInventoryItemData InventoryItemData, int32 SourceSlotIndex, int32 TargetSlotIndex)
{
	const bool bWasSuccessful = Super::AddItemToIndex(InventoryItemData, SourceSlotIndex, TargetSlotIndex);

	IPlayerInterface::Execute_UpdateInventorySlot(GetOwner(), ContainerType, TargetSlotIndex, GetItemAtIndex(TargetSlotIndex));
	
	return bWasSuccessful;
}

bool UHotbarComponent::RemoveItemAtIndex(int32 SlotIndex)
{
	const bool bWasSuccessful = Super::RemoveItemAtIndex(SlotIndex);

	IPlayerInterface::Execute_ResetInventorySlot(GetOwner(), ContainerType, SlotIndex);

	return bWasSuccessful;
}
