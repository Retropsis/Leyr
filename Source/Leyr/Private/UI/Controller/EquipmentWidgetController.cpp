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

void UEquipmentWidgetController::Equip(const FInventoryItemData& ItemData, const FGameplayTag InputTag)
{
	const FInventoryItemData Item = ULeyrAbilitySystemLibrary::FindItemDataByRowName(this, ItemData.Name);
	FBaseGameplayTags GameplayTags = FBaseGameplayTags::Get();
	FGameplayTag Slot = Item.EquipmentSlot;

	if(Slot.MatchesTagExact(GameplayTags.Equipment_ActionSlot))
	{
		// Slot = GameplayTags.EquipmentSlotToInputTags.Contains(InputTag) ? GameplayTags.EquipmentSlotToInputTags[InputTag] : FGameplayTag();
		// if(InputTag.MatchesTagExact(GameplayTags.InputTag_LMB)) Slot = GameplayTags.Equipment_ActionSlot_1;
		// if(InputTag.MatchesTagExact(GameplayTags.InputTag_RMB)) Slot = GameplayTags.Equipment_ActionSlot_2;
		// if(InputTag.MatchesTagExact(GameplayTags.InputTag_1)) Slot = GameplayTags.Equipment_ActionSlot_3;
		for (TTuple<FGameplayTag, FGameplayTag> Pair : GameplayTags.InputTagsToEquipmentSlots)
		{
			if(Pair.Key.MatchesTagExact(InputTag)) Slot = Pair.Value;
		}
	}
	EquippedItems.Add(Slot, Item);
	OnItemEquipped.Broadcast(Slot, Item);
}

void UEquipmentWidgetController::Unequip(const FGameplayTag InputTag)
{
	FBaseGameplayTags GameplayTags = FBaseGameplayTags::Get();
	FGameplayTag Slot = FGameplayTag();
	// FGameplayTag Slot = GameplayTags.InputTagsToEquipmentSlots.Contains(InputTag) ? GameplayTags.InputTagsToEquipmentSlots[InputTag] : FGameplayTag();
	for (TTuple<FGameplayTag, FGameplayTag> Pair : GameplayTags.InputTagsToEquipmentSlots)
	{
		if(Pair.Key.MatchesTagExact(InputTag)) Slot = Pair.Value;
	}
	EquippedItems.Remove(Slot);
	OnItemUnequipped.Broadcast(Slot);
}
