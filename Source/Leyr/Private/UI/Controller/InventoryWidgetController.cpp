// @ Retropsis 2024-2025.

#include "UI/Controller/InventoryWidgetController.h"
#include "AbilitySystem/LeyrAbilitySystemLibrary.h"
#include "Game/BaseGameplayTags.h"
#include "Inventory/InventoryComponent.h"

void UInventoryWidgetController::BindCallbacksToDependencies()
{
	checkf(InventoryComponent, TEXT("InventoryComponent is NULL, please check [%s]"), *GetName());
	InventoryComponent->OnItemUpdated.AddDynamic(this, &UInventoryWidgetController::HandleItemUpdated);
}

void UInventoryWidgetController::EquipButtonPressed(FInventoryItemData ItemData, const FGameplayTag& InputTag)
{
	if(ItemData.ID == 0) return;
	
	//TODO: Need to be unique ID in case there are same items, might need a ref of the inventory slot
	if (ClearEquipButtonByInputTag(InputTag, ItemData.ID)) return; 
	if (ReplaceInputTag(ItemData, InputTag)) return;
	AssignInputTag(ItemData, InputTag);
}

void UInventoryWidgetController::LootButtonPressed(int32 SourceSlotIndex)
{
	if(!bContainerIsOpen) return;
	if(UInventoryComponent* ContainerComponent = ULeyrAbilitySystemLibrary::GetContainerComponent(this))
	{
		ContainerComponent->TransferItemToEmptySlot(InventoryComponent, SourceSlotIndex);
	}
}

void UInventoryWidgetController::LootAllButtonPressed()
{
	if(!bContainerIsOpen) return;
	if(UInventoryComponent* ContainerComponent = ULeyrAbilitySystemLibrary::GetContainerComponent(this))
	{
		for (int i = 0; i < ContainerComponent->Items.Num(); ++i)
		{
			if(ContainerComponent->Items[i].ID != 0)
			{
				ContainerComponent->TransferItemToEmptySlot(InventoryComponent, i);
			}
		}
	}
}

void UInventoryWidgetController::ClearInputTag(FInventoryItemData ItemData, const FGameplayTag& InputTag)
{
	ULeyrAbilitySystemLibrary::UpdateItemAbilities(this, AbilitySystemComponent, ItemData, InputTag, true);
	ULeyrAbilitySystemLibrary::UpdateMonkAbilities(this, AbilitySystemComponent, InputTag, false);
	EquippedItemAbilities.Remove(InputTag);
	OnInputRemoved.Broadcast(InputTag);
	InventoryComponent->UpdateItemInputTag(ItemData, FBaseGameplayTags::Get().Abilities_None);
}

void UInventoryWidgetController::AssignInputTag(FInventoryItemData ItemData, const FGameplayTag& InputTag)
{
	ULeyrAbilitySystemLibrary::UpdateMonkAbilities(this, AbilitySystemComponent, InputTag, true);
	ULeyrAbilitySystemLibrary::UpdateItemAbilities(this, AbilitySystemComponent, ItemData, InputTag, false);
	EquippedItemAbilities.Add(InputTag, ItemData);
	OnInputAssigned.Broadcast(ItemData, InputTag);
	InventoryComponent->UpdateItemInputTag(ItemData, InputTag);
}

bool UInventoryWidgetController::ReplaceInputTag(FInventoryItemData ItemData, const FGameplayTag& InputTag)
{
	for (TTuple<FGameplayTag, FInventoryItemData> EquippedAbility : EquippedItemAbilities)
	{
		if(EquippedAbility.Value.ID == ItemData.ID)
		{
			ULeyrAbilitySystemLibrary::ReplaceAbilityInputTag(this, AbilitySystemComponent, ItemData, InputTag, EquippedAbility.Key);
			ULeyrAbilitySystemLibrary::UpdateMonkAbilities(this, AbilitySystemComponent, EquippedAbility.Key, false);
			EquippedItemAbilities.Remove(EquippedAbility.Key);
			EquippedItemAbilities.Add(InputTag, ItemData);
			OnInputRemoved.Broadcast(EquippedAbility.Key);
			OnInputAssigned.Broadcast(ItemData, InputTag);
			InventoryComponent->UpdateItemInputTag(ItemData, InputTag);
			return true;
		}
	}
	return false;
}

void UInventoryWidgetController::HandleItemUpdated(EContainerType ContainerType, int32 SlotIndex, FInventoryItemData Item)
{
	if (Item.Quantity == 0)
	{
		ClearEquipButtonByItemData(Item);
	}
}

void UInventoryWidgetController::ClearEquipButtonByItemData(const FInventoryItemData& ItemData)
{
	for (TTuple<FGameplayTag, FInventoryItemData> EquippedAbility : EquippedItemAbilities)
	{
		if(EquippedAbility.Value.ID == ItemData.ID)
		{
			ClearInputTag(ItemData, EquippedAbility.Key);
			return;
		}
	}
}

bool UInventoryWidgetController::ClearEquipButtonByInputTag(const FGameplayTag InputTag, const int32 ItemID)
{
	if(const FInventoryItemData* ItemToRemove = EquippedItemAbilities.Find(InputTag))
	{
		ClearInputTag(*ItemToRemove, InputTag);
		return ItemID == ItemToRemove->ID;
	}
	return false;
}
