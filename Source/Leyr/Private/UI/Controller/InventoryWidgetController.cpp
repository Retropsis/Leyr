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
	const FBaseGameplayTags& GameplayTags = FBaseGameplayTags::Get();
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
	
	UItemData* ItemDataAsset = ItemData.Asset.LoadSynchronous();
	for (TSubclassOf<UGameplayEffect> Effect : ItemDataAsset->Effects)
	{
		ApplyExecuteEffectToSelf(Effect.GetDefaultObject(), ItemData.Asset.Get(), Slot);
	}
	MakeAndApplyEffectToSelf(ItemData.Asset.Get(), Slot, ItemDataAsset->Modifiers);
}

void UInventoryWidgetController::Equip(const FInventoryItemData& ItemData)
{
	const FInventoryItemData Item = ULeyrAbilitySystemLibrary::FindItemDataByRowName(this, ItemData.Name); // Why do I do this ????
	const FBaseGameplayTags& GameplayTags = FBaseGameplayTags::Get();
	FGameplayTag Slot = Item.EquipmentSlot;
	
	for (TTuple<FGameplayTag, FInventoryItemData> EquippedItem : EquippedItems)
	{
		if (Slot.MatchesTagExact(EquippedItem.Key))
		{
			if (EquippedItems.Contains(Slot) && ItemData.Asset == EquippedItem.Value.Asset)
			{
				EquippedItems.Remove(Slot);
				OnItemUnequipped.Broadcast(Slot);
				RemoveActiveGameplayEffect(Slot);
				return;
			}
		}
	}
	EquippedItems.Add(Slot, Item);
	OnItemEquipped.Broadcast(Slot, Item);

	UItemData* ItemDataAsset = ItemData.Asset.LoadSynchronous();
	for (TSubclassOf<UGameplayEffect> Effect : ItemDataAsset->Effects)
	{
		ApplyExecuteEffectToSelf(Effect.GetDefaultObject(), ItemData.Asset.Get(), Slot);
	}
	MakeAndApplyEffectToSelf(ItemData.Asset.Get(), Slot, ItemDataAsset->Modifiers);
}

void UInventoryWidgetController::Unequip(FGameplayTag InputTag)
{
	const FBaseGameplayTags& GameplayTags = FBaseGameplayTags::Get();
	FGameplayTag Slot = FGameplayTag();
	for (TTuple<FGameplayTag, FGameplayTag> Pair : GameplayTags.InputTagsToEquipmentSlots)
	{
		if(Pair.Key.MatchesTagExact(InputTag)) Slot = Pair.Value;
	}
	EquippedItems.Remove(Slot);
	OnItemUnequipped.Broadcast(Slot);
	GEngine->AddOnScreenDebugMessage(9789444, 5.f, FColor::Magenta, "UInventoryWidgetController::Unequip");
	RemoveActiveGameplayEffect(Slot);
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

void UInventoryWidgetController::AssignInputTag(const FInventoryItemData& ItemData, const FGameplayTag& InputTag)
{
	ULeyrAbilitySystemLibrary::UpdateMonkAbilities(this, AbilitySystemComponent, InputTag, true);
	ULeyrAbilitySystemLibrary::UpdateItemAbilities(this, AbilitySystemComponent, ItemData, InputTag, false);
	EquippedItemAbilities.Add(InputTag, ItemData);
	OnInputAssigned.Broadcast(ItemData, InputTag);
	Assign(ItemData, InputTag);
	InventoryComponent->UpdateItemInputTag(ItemData, InputTag);
}

void UInventoryWidgetController::ApplyExecuteEffectToSelf(UGameplayEffect* EffectToApply, const UObject* SourceObject, const FGameplayTag& EquipmentSlot, const int32 Level)
{
	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(SourceObject);
	
	UTargetTagsGameplayEffectComponent& AssetTagsComponent = EffectToApply->FindOrAddComponent<UTargetTagsGameplayEffectComponent>();
	FInheritedTagContainer InheritedTagContainer;
	InheritedTagContainer.Added.AddTag(EquipmentSlot);
	AssetTagsComponent.SetAndApplyTargetTagChanges(InheritedTagContainer);

	const FActiveGameplayEffectHandle ActiveEffectHandle = AbilitySystemComponent->ApplyGameplayEffectToSelf(EffectToApply, Level, EffectContext);
	EquippedEffects.Add(EquipmentSlot, ActiveEffectHandle);
}

void UInventoryWidgetController::MakeAndApplyEffectToSelf(const UObject* SourceObject, const FGameplayTag& EquipmentSlot, TArray<FGameplayModifierInfo> Modifiers, const int32 Level)
{
	const FBaseGameplayTags& GameplayTags = FBaseGameplayTags::Get();
	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(SourceObject);

	FString TagName = FString::Printf(TEXT("%s"), *EquipmentSlot.ToString());
	UGameplayEffect* Effect = NewObject<UGameplayEffect>(GetTransientPackage(), FName(TagName));

	Effect->DurationPolicy = EGameplayEffectDurationType::Infinite;
	
	UTargetTagsGameplayEffectComponent& AssetTagsComponent = Effect->FindOrAddComponent<UTargetTagsGameplayEffectComponent>();
	FInheritedTagContainer InheritedTagContainer;
	InheritedTagContainer.Added.AddTag(EquipmentSlot);
	AssetTagsComponent.SetAndApplyTargetTagChanges(InheritedTagContainer);

	for (FGameplayModifierInfo Modifier : Modifiers)
	{
		Effect->Modifiers.Add(Modifier);
	}

	if (const FGameplayEffectSpec* MutableSpec = new FGameplayEffectSpec(Effect, EffectContext, Level))
	{
		const FActiveGameplayEffectHandle ActiveEffectHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*MutableSpec, AbilitySystemComponent);
		EquippedEffects.Add(EquipmentSlot, ActiveEffectHandle);
	}
}

void UInventoryWidgetController::RemoveActiveGameplayEffect(FGameplayTag EquipmentSlot)
{
	if(EquippedEffects.IsEmpty()) return;
	
	const FActiveGameplayEffectHandle* HandleToRemove = EquippedEffects.Find(EquipmentSlot);
	AbilitySystemComponent->RemoveActiveGameplayEffect(*HandleToRemove);
	// FGameplayEffectQuery Query;
	// Query.EffectTagQuery.
	// AbilitySystemComponent->RemoveActiveEffects()
	
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

UItemData* UInventoryWidgetController::HasCompatibleItemCostInAmmunitionSlot(const FGameplayTag CostTag)
{
	if (const FInventoryItemData* ItemData = EquippedItems.Find(FBaseGameplayTags::Get().Equipment_Ammunition))
	{
		if(UItemData* Asset = ItemData->Asset.LoadSynchronous())
		{
			return Asset->CostTag.MatchesTagExact(CostTag) ? Asset : nullptr;
		}
	}
	return nullptr;
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
