// @ Retropsis 2024-2025.

#include "UI/Controller/EquipmentWidgetController.h"
#include "GameplayTagContainer.h"
#include "AbilitySystem/LeyrAbilitySystemLibrary.h"
#include "Game/BaseGameplayTags.h"
#include "Inventory/InventoryComponent.h"

void UEquipmentWidgetController::BroadcastInitialValues()
{
	Super::BroadcastInitialValues();
}

void UEquipmentWidgetController::BindCallbacksToDependencies()
{
	Super::BindCallbacksToDependencies();
}

void UEquipmentWidgetController::Assign(const FInventoryItemData& ItemData, const FGameplayTag InputTag)
{
	const FInventoryItemData Item = ULeyrAbilitySystemLibrary::FindItemDataByRowName(this, ItemData.Name);
	FBaseGameplayTags GameplayTags = FBaseGameplayTags::Get();
	FGameplayTag Slot = Item.EquipmentSlot;

	if(Slot.MatchesTagExact(GameplayTags.Equipment_ActionSlot))
	{
		for (TTuple<FGameplayTag, FGameplayTag> Pair : GameplayTags.InputTagsToEquipmentSlots)
		{
			if(Pair.Key.MatchesTagExact(InputTag)) Slot = Pair.Value;
		}
	}
	
	EquippedItems.Add(Slot, Item);
	OnItemEquipped.Broadcast(Slot, Item);
}

void UEquipmentWidgetController::Equip(const FInventoryItemData& ItemData)
{
	const FInventoryItemData Item = ULeyrAbilitySystemLibrary::FindItemDataByRowName(this, ItemData.Name);
	FBaseGameplayTags GameplayTags = FBaseGameplayTags::Get();
	FGameplayTag Slot = Item.EquipmentSlot;
	
	for (TTuple<FGameplayTag, FInventoryItemData> EquippedItem : EquippedItems)
	{
		if (Slot.MatchesTagExact(EquippedItem.Key))
		{
			if (EquippedItems.Contains(Slot) && ItemData.Asset == EquippedItem.Value.Asset)
			{
				EquippedItems.Remove(Slot);
				OnItemUnequipped.Broadcast(Slot);
				return;
			}
		}
	}
	EquippedItems.Add(Slot, Item);
	OnItemEquipped.Broadcast(Slot, Item);
}

void UEquipmentWidgetController::Unequip(const FGameplayTag InputTag)
{
	FBaseGameplayTags GameplayTags = FBaseGameplayTags::Get();
	FGameplayTag Slot = FGameplayTag();
	for (TTuple<FGameplayTag, FGameplayTag> Pair : GameplayTags.InputTagsToEquipmentSlots)
	{
		if(Pair.Key.MatchesTagExact(InputTag)) Slot = Pair.Value;
	}
	EquippedItems.Remove(Slot);
	OnItemUnequipped.Broadcast(Slot);
}
