// @ Retropsis 2024-2025.

#include "UI/Controller/InventoryWidgetController.h"
#include "AbilitySystem/LeyrAbilitySystemLibrary.h"
#include "Game/BaseGameplayTags.h"
#include "Inventory/InventoryComponent.h"

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
		
		if(ItemToRemove->ID == ItemData.ID) return;
	}
	// Replace
	for (TTuple<FGameplayTag, FInventoryItemData> EquippedAbility : EquippedItemAbilities)
	{
		if(EquippedAbility.Value.ID == ItemData.ID)
		{
			ULeyrAbilitySystemLibrary::ReplaceAbilityInputTag(this, AbilitySystemComponent, ItemData, InputTag, EquippedAbility.Key);
			EquippedItemAbilities.Remove(EquippedAbility.Key);
			EquippedItemAbilities.Add(InputTag, ItemData);
			OnInputAssigned.Broadcast(ItemData.ID, InputTag);
			return;
		}
	}
	// New Assignment
	ULeyrAbilitySystemLibrary::GiveItemAbilities(this, AbilitySystemComponent, ItemData, InputTag);
	ULeyrAbilitySystemLibrary::ClearInputFromMonkAbilities(this, AbilitySystemComponent, InputTag);
	EquippedItemAbilities.Add(InputTag, ItemData);
	OnInputAssigned.Broadcast(ItemData.ID, InputTag);
}
