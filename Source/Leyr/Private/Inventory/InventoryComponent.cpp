// @ Retropsis 2024-2025.

#include "Inventory/InventoryComponent.h"
#include "Data/ItemData.h"
#include "Interaction/PlayerInterface.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UInventoryComponent::ServerAddItem_Implementation(FInventoryItemData ItemToAdd)
{
	AddItem(ItemToAdd);
}

// bItemStackWasSplit is for splitting a stack, set it to true to skip stacking and add the new split in a new slot
void UInventoryComponent::AddItem(FInventoryItemData& ItemToAdd, const bool bItemStackWasSplit)
{
	UItemData* Item = ItemToAdd.Asset.LoadSynchronous();
	const int32 ItemID = ItemToAdd.ID;
	int32 ItemQuantityToAdd = ItemToAdd.Quantity;
	
	if(Item && Item->bIsStackable && !bItemStackWasSplit)
	{
		for (int i = 0; i < Items.Num(); ++i)
		{
			if (Items[i].ID == ItemID && Items[i].Quantity < Items[i].StackSize)
			{
				const int32 Amount = FMath::Min(ItemQuantityToAdd, Items[i].StackSize - Items[i].Quantity);
				Items[i].Quantity += Amount;
				ItemQuantityToAdd -= Amount;
				ItemToAdd.Quantity = ItemQuantityToAdd;
				UpdateInventorySlotUI(i, Items[i]);
				if(ItemQuantityToAdd == 0) break;
			}
		}
	}
	int32 EmptySlotIndex;
	if(FindEmptySlot(EmptySlotIndex) && ItemQuantityToAdd > 0 && EmptySlotIndex != INDEX_NONE)
	{
		Items[EmptySlotIndex] = ItemToAdd;
		UpdateInventorySlotUI(EmptySlotIndex, ItemToAdd);
	}
}

bool UInventoryComponent::AddItemToIndex(FInventoryItemData InventoryItemData, int32 SourceSlotIndex, int32 TargetSlotIndex)
{
	if(IsSlotEmpty(TargetSlotIndex))
	{
		Items[TargetSlotIndex] = InventoryItemData;
		return true;
	}
	else
	{
		// Stacking Items
	}
	return false;
}

bool UInventoryComponent::RemoveItemAtIndex(int32 SlotIndex)
{
	Items[SlotIndex] = FInventoryItemData();
	return true;
}

void UInventoryComponent::OnSlotDrop_Implementation(UInventoryComponent* SourceInventory, int32 SourceSlotIndex, int32 TargetSlotIndex)
{
	HandleOnSlotDrop(SourceInventory, SourceSlotIndex, TargetSlotIndex);
}

void UInventoryComponent::TransferItem(UInventoryComponent* TargetInventory, int32 SourceSlotIndex, int32 TargetSlotIndex)
{
	if(IsSlotEmpty(SourceSlotIndex) || (SourceSlotIndex == TargetSlotIndex && TargetInventory == this)) return;

	if(TargetInventory)
	{
		const FInventoryItemData ItemData = GetItemAtIndex(SourceSlotIndex);
		if (TargetInventory->AddItemToIndex(ItemData, SourceSlotIndex, TargetSlotIndex))
		{
			RemoveItemAtIndex(SourceSlotIndex);
		}
	}
}

void UInventoryComponent::TransferItemToEmptySlot(UInventoryComponent* TargetInventory, int32 SourceSlotIndex)
{
	if(TargetInventory == this) return;
	
	int32 EmptySlotIndex;
	if(TargetInventory && TargetInventory->FindEmptySlot(EmptySlotIndex))
	{
		FInventoryItemData ItemData = GetItemAtIndex(SourceSlotIndex);
		TargetInventory->AddItem(ItemData);
		RemoveItemAtIndex(SourceSlotIndex);
	}
}

bool UInventoryComponent::IsSlotEmpty(int32 SlotIndex)
{
	checkf(Items.IsValidIndex(SlotIndex), TEXT("Attempting to reach out of bounds index in %s"), *GetNameSafe(this));
	return Items[SlotIndex].ID == 0;
}

FInventoryItemData UInventoryComponent::GetItemAtIndex(int32 SlotIndex)
{
	return Items.IsValidIndex(SlotIndex) ? Items[SlotIndex] : FInventoryItemData();
}

void UInventoryComponent::UpdateInventorySlotUI(int32 SlotIndex, const FInventoryItemData& ItemData) const
{
	// TODO: Remainder if custom behaviour is needed, don't forget to break
	switch (ContainerType)
	{
	case EContainerType::Inventory:
	case EContainerType::Hotbar:
		IPlayerInterface::Execute_UpdateInventorySlot(GetOwner(), ContainerType, SlotIndex, ItemData);
		break;
	case EContainerType::Container:
		break;
	case EContainerType::Equipment:
		break;
	default: ;
	}
}

void UInventoryComponent::ResetInventorySlotUI(int32 SlotIndex, const FInventoryItemData& ItemData) const
{
	// TODO: Remainder if custom behaviour is needed, don't forget to break
	switch (ContainerType)
	{
	case EContainerType::Inventory:
	case EContainerType::Hotbar:
		IPlayerInterface::Execute_ResetInventorySlot(GetOwner(), ContainerType, SlotIndex);
		break;
	case EContainerType::Container:
		break;
	case EContainerType::Equipment:
		break;
	default: ;
	}
}

void UInventoryComponent::SetInventorySize(int32 Size)
{
	Items.SetNum(Size, EAllowShrinking::No);
}

bool UInventoryComponent::FindEmptySlot(int32& EmptySlotIndex)
{
	for (int i = 0; i < Items.Num(); ++i)
	{
		if (Items[i].ID == 0)
		{
			EmptySlotIndex = i;
			return true;
		}
	}
	return false;
}

/*
 * Custom Functions
 */
bool UInventoryComponent::UseItem(UItemData* Asset, int32 Amount)
{
	for (int i = 0; i < Items.Num(); ++i)
	{
		if (Items[i].Asset == Asset && Items[i].Quantity >= Amount)
		{
			Items[i].Quantity -= Amount;
			if (Items[i].Quantity > 0 || !Asset->bRemoveStackIfEmpty)
			{
				UpdateInventorySlotUI(i, Items[i]);
			}
			else
			{
				ResetInventorySlotUI(i, Items[i]);
				RemoveItemAtIndex(i);
			}
			OnItemQuantityUpdated.Broadcast(Items[i]);
			return true;
		}
	}
	return false;
}