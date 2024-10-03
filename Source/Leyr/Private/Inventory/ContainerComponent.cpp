// @ Retropsis 2024-2025.

#include "Inventory/ContainerComponent.h"
#include "Interaction/PlayerInterface.h"
#include "Inventory/Container/Container.h"

UContainerComponent::UContainerComponent()
{
	ContainerType = EContainerType::Container;
}

void UContainerComponent::HandleOnSlotDrop(UInventoryComponent* SourceInventory, int32 SourceSlotIndex, int32 TargetSlotIndex)
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

bool UContainerComponent::AddItemToIndex(FInventoryItemData InventoryItemData, int32 SourceSlotIndex, int32 TargetSlotIndex)
{
	const bool bWasSuccessful = Super::AddItemToIndex(InventoryItemData, SourceSlotIndex, TargetSlotIndex);

	if (AContainer* Container = IPlayerInterface::Execute_GetContainer(GetOwner()))
	{
		Container->ServerForEachActorUpdateInventorySlot(ContainerType, TargetSlotIndex, GetItemAtIndex(TargetSlotIndex));
	}
	
	return bWasSuccessful;
}

bool UContainerComponent::RemoveItemAtIndex(int32 SlotIndex)
{
	const bool bWasSuccessful = Super::RemoveItemAtIndex(SlotIndex);

	if (AContainer* Container = IPlayerInterface::Execute_GetContainer(GetOwner()))
	{
		if(Container->ContainerSubType == EContainerSubType::PlayerBag)
		{
			Items.RemoveAt(SlotIndex);
			Container->ServerUpdateContainer();
		}
		else
		{
			Container->ServerForEachActorResetInventorySlot(ContainerType, SlotIndex);
		}
	}

	return bWasSuccessful;
}
