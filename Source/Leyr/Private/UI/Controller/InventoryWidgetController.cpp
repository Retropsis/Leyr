// @ Retropsis 2024-2025.

#include "UI/Controller/InventoryWidgetController.h"
#include "AbilitySystem/LeyrAbilitySystemLibrary.h"
#include "Game/BaseGameplayTags.h"
#include "Inventory/InventoryComponent.h"

void UInventoryWidgetController::BindCallbacksToDependencies()
{
	if(InventoryComponent) InventoryComponent->OnItemUpdated.AddDynamic(this, &UInventoryWidgetController::HandleItemUpdated);
}

void UInventoryWidgetController::EquipButtonPressed(FInventoryItemData ItemData, const FGameplayTag& InputTag)
{
	if(ItemData.ID == 0) return;
	
	//TODO: Need to be unique ID in case there are same items, might need a ref of the inventory slot
	//TODO: Handling bare fists here ?	
	if(FInventoryItemData* ItemToRemove = EquippedItemAbilities.Find(InputTag))
	{
		// Clear
		ULeyrAbilitySystemLibrary::RemoveItemAbilities(this, AbilitySystemComponent, *ItemToRemove, InputTag);
		ULeyrAbilitySystemLibrary::AssignMonkAbilities(this, AbilitySystemComponent, InputTag);
		EquippedItemAbilities.Remove(InputTag);
		OnInputAssigned.Broadcast(ItemToRemove->ID, FBaseGameplayTags::Get().Abilities_None);
		if(InventoryComponent) InventoryComponent->UpdateItemInputTag(*ItemToRemove, FBaseGameplayTags::Get().Abilities_None);
		
		if(ItemToRemove->ID == ItemData.ID) return; // We required same input for same item so we cleared and leave
	}
	// Replace
	for (TTuple<FGameplayTag, FInventoryItemData> EquippedAbility : EquippedItemAbilities)
	{
		if(EquippedAbility.Value.ID == ItemData.ID)
		{
			ULeyrAbilitySystemLibrary::ReplaceAbilityInputTag(this, AbilitySystemComponent, ItemData, InputTag, EquippedAbility.Key);
			ULeyrAbilitySystemLibrary::AssignMonkAbilities(this, AbilitySystemComponent, EquippedAbility.Key);
			EquippedItemAbilities.Remove(EquippedAbility.Key);
			EquippedItemAbilities.Add(InputTag, ItemData);
			OnInputAssigned.Broadcast(ItemData.ID, InputTag);
			if(InventoryComponent) InventoryComponent->UpdateItemInputTag(ItemData, InputTag);
			return;
		}
	}
	// New Assignment
	ULeyrAbilitySystemLibrary::ClearInputFromMonkAbilities(this, AbilitySystemComponent, InputTag);
	ULeyrAbilitySystemLibrary::GiveItemAbilities(this, AbilitySystemComponent, ItemData, InputTag);
	EquippedItemAbilities.Add(InputTag, ItemData);
	OnInputAssigned.Broadcast(ItemData.ID, InputTag);
	if(InventoryComponent) InventoryComponent->UpdateItemInputTag(ItemData, InputTag);
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
			ULeyrAbilitySystemLibrary::RemoveItemAbilities(this, AbilitySystemComponent, ItemData, EquippedAbility.Key);
			ULeyrAbilitySystemLibrary::AssignMonkAbilities(this, AbilitySystemComponent, EquippedAbility.Key);
			EquippedItemAbilities.Remove(EquippedAbility.Key);
			OnInputAssigned.Broadcast(ItemData.ID, FBaseGameplayTags::Get().Abilities_None);
			if(InventoryComponent) InventoryComponent->UpdateItemInputTag(ItemData, FBaseGameplayTags::Get().Abilities_None);
			return;
		}
	}
}

void UInventoryWidgetController::ClearEquipButtonByInputTag(const FGameplayTag InputTag)
{
	if(FInventoryItemData* ItemToRemove = EquippedItemAbilities.Find(InputTag))
	{
		ULeyrAbilitySystemLibrary::RemoveItemAbilities(this, AbilitySystemComponent, *ItemToRemove, InputTag);
		ULeyrAbilitySystemLibrary::AssignMonkAbilities(this, AbilitySystemComponent, InputTag);
		EquippedItemAbilities.Remove(InputTag);
		OnInputAssigned.Broadcast(ItemToRemove->ID, FBaseGameplayTags::Get().Abilities_None);
		if(InventoryComponent) InventoryComponent->UpdateItemInputTag(*ItemToRemove, FBaseGameplayTags::Get().Abilities_None);
	}
}
