// @ Retropsis 2024-2025.

#include "UI/Controller/InventoryWidgetController.h"

#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"
#include "AbilitySystem/LeyrAbilitySystemLibrary.h"
#include "Game/BaseGameplayTags.h"
#include "GameplayEffectComponents/TargetTagsGameplayEffectComponent.h"
#include "Inventory/InventoryComponent.h"

void UInventoryWidgetController::BindCallbacksToDependencies()
{
	checkf(InventoryComponent, TEXT("InventoryComponent is NULL, please check [%s]"), *GetName());
	InventoryComponent->OnItemUpdated.AddDynamic(this, &UInventoryWidgetController::HandleItemUpdated);
}

void UInventoryWidgetController::AssignButtonPressed(FInventoryItemData ItemData, const FGameplayTag& InputTag)
{
	if(ItemData.ID == 0) return;
	FBaseGameplayTags GameplayTags = FBaseGameplayTags::Get();

	if(ItemData.EquipmentSlot.MatchesTagExact(GameplayTags.Equipment_ActionSlot))
	{
		//TODO: Need to be unique ID in case there are same items, might need a ref of the inventory slot
        if (ClearEquipButtonByInputTag(InputTag, ItemData.ID)) return; 
        if (ReplaceInputTag(ItemData, InputTag)) return;
        AssignInputTag(ItemData, InputTag);
	}
}

void UInventoryWidgetController::EquipButtonPressed(FInventoryItemData ItemData)
{
	// OnEquipmentAssigned.Broadcast(ItemData);
	Equip(ItemData);
}

void UInventoryWidgetController::Assign(const FInventoryItemData& ItemData, FGameplayTag InputTag)
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

void UInventoryWidgetController::Equip(const FInventoryItemData& ItemData)
{
	const FInventoryItemData Item = ULeyrAbilitySystemLibrary::FindItemDataByRowName(this, ItemData.Name); // Why do I do this ????
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

	TArray<TSubclassOf<UGameplayEffect>> Effects = ItemData.Asset.LoadSynchronous()->Effects;
	for (TSubclassOf<UGameplayEffect> Effect : Effects)
	{
		MakeAndApplyExecuteEffectToSelf(Effect.GetDefaultObject(), ItemData.Asset.Get());
	}
}

void UInventoryWidgetController::Unequip(FGameplayTag InputTag)
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
	Unequip(InputTag);
	InventoryComponent->UpdateItemInputTag(ItemData, FBaseGameplayTags::Get().InputTag_None);
}

void UInventoryWidgetController::AssignInputTag(FInventoryItemData ItemData, const FGameplayTag& InputTag)
{
	ULeyrAbilitySystemLibrary::UpdateMonkAbilities(this, AbilitySystemComponent, InputTag, true);
	ULeyrAbilitySystemLibrary::UpdateItemAbilities(this, AbilitySystemComponent, ItemData, InputTag, false);
	EquippedItemAbilities.Add(InputTag, ItemData);
	OnInputAssigned.Broadcast(ItemData, InputTag);
	Assign(ItemData, InputTag);
	InventoryComponent->UpdateItemInputTag(ItemData, InputTag);
}

void UInventoryWidgetController::MakeAndApplyExecuteEffectToSelf(UGameplayEffect* EffectToApply, const UObject* SourceObject, const FGameplayTag& TagToApply, const int32 Level) const
{
	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(SourceObject);
	
	UTargetTagsGameplayEffectComponent& AssetTagsComponent = EffectToApply->FindOrAddComponent<UTargetTagsGameplayEffectComponent>();
	FInheritedTagContainer InheritedTagContainer;
	InheritedTagContainer.Added.AddTag(TagToApply);
	AssetTagsComponent.SetAndApplyTargetTagChanges(InheritedTagContainer);

	AbilitySystemComponent->ApplyGameplayEffectToSelf(EffectToApply, Level, EffectContext);
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
			
			Assign(ItemData, InputTag);
			Unequip(EquippedAbility.Key);
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
