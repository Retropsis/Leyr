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

/*
 * Assigning
 */
void UInventoryWidgetController::AssignButtonPressed(const FInventoryItemData ItemData, const FGameplayTag InputTag)
{
	if(ItemData.Asset.Get() == nullptr) return;
	
	const FBaseGameplayTags& GameplayTags = FBaseGameplayTags::Get();

	if(ItemData.EquipmentSlot.MatchesTagExact(GameplayTags.Equipment_ActionSlot))
	{
		FGameplayTag InputToAssign = InputTag;
		FGameplayTag SlotToEquip = GameplayTags.EquipmentSlotToInputTags[InputTag];
		FEquippedItem ItemToEquip = FEquippedItem{ ItemData };
		
		FGameplayTag InputToClear = FGameplayTag();
		FGameplayTag SlotToUnequip = FGameplayTag();
		FEquippedItem ItemToClearFromInput = FEquippedItem();
		bool bShouldClear = false;

		// INPUT/SLOT ALREADY ASSIGNED
		if(const FEquippedItem* EquippedItem = EquippedItems.Find(SlotToEquip))
		{
			ItemToClearFromInput = *EquippedItem;
			InputToClear = InputToAssign;
			// SlotToUnequip = GameplayTags.EquipmentSlotToInputTags[InputToClear];
			bShouldClear = true;
		}

		// ITEM ALREADY ASSIGNED
		for (TTuple<FGameplayTag, FEquippedItem> EquippedItem : EquippedItems)
		{
			if (EquippedItem.Value.ItemData.Asset.Get() == ItemData.Asset.Get())
			{
				SlotToUnequip = EquippedItem.Key;
				// InputToClear = GameplayTags.EquipmentSlotToInputTags[SlotToUnequip];
				InputToClear = InputToAssign;
				bShouldClear = true;
				break;
			}
		}

		// 1. Clear
		if(bShouldClear)
		{
			Clear(InputToClear, SlotToUnequip);
		}
		
		if (ItemToClearFromInput.ItemData.Asset.Get() == ItemToEquip.ItemData.Asset.Get() && InputToAssign.MatchesTagExact(InputToClear)) // Clear
		{
			GEngine->AddOnScreenDebugMessage(48950, 5.f, FColor::Green, FString::Printf(TEXT("Clear")));
			ULeyrAbilitySystemLibrary::UpdateItemAbilities(this, AbilitySystemComponent, ItemData, InputToAssign, true);
			ULeyrAbilitySystemLibrary::UpdateMonkAbilities(this, AbilitySystemComponent, InputToAssign, false);
			return;
		}
		if (ItemToClearFromInput.ItemData.Asset.Get() == nullptr && !InputToClear.IsValid()) // Assign new input to new item
		{
			GEngine->AddOnScreenDebugMessage(48954, 5.f, FColor::Green, FString::Printf(TEXT("Assign")));
			ULeyrAbilitySystemLibrary::UpdateMonkAbilities(this, AbilitySystemComponent, InputToAssign, true);
			ULeyrAbilitySystemLibrary::UpdateItemAbilities(this, AbilitySystemComponent, ItemData, InputToAssign, false);
		}
		else if (ItemToClearFromInput.ItemData.Asset.Get() == nullptr && SlotToUnequip.IsValid()) // Keep Item, Change Input
		{
			GEngine->AddOnScreenDebugMessage(48951, 5.f, FColor::Green, FString::Printf(TEXT("Keep Item, Change Input")));
			ULeyrAbilitySystemLibrary::UpdateMonkAbilities(this, AbilitySystemComponent, InputToAssign, true);
			ULeyrAbilitySystemLibrary::ReplaceAbilityInputTag(this, AbilitySystemComponent, ItemData, InputToAssign, GameplayTags.EquipmentSlotToInputTags[SlotToUnequip]);
			ULeyrAbilitySystemLibrary::UpdateMonkAbilities(this, AbilitySystemComponent, GameplayTags.EquipmentSlotToInputTags[SlotToUnequip], false);
		}
		else if (ItemToClearFromInput.ItemData.Asset.Get() != ItemToEquip.ItemData.Asset.Get() && SlotToUnequip.IsValid()) // Steal already assigned input to replace its own input
		{
			GEngine->AddOnScreenDebugMessage(48952, 5.f, FColor::Green, FString::Printf(TEXT("Steal")));
			ULeyrAbilitySystemLibrary::UpdateItemAbilities(this, AbilitySystemComponent, ItemToClearFromInput.ItemData, InputToAssign, true);
			ULeyrAbilitySystemLibrary::UpdateMonkAbilities(this, AbilitySystemComponent, GameplayTags.EquipmentSlotToInputTags[SlotToUnequip], false);
			ULeyrAbilitySystemLibrary::ReplaceAbilityInputTag(this, AbilitySystemComponent, ItemData, InputToAssign, GameplayTags.EquipmentSlotToInputTags[SlotToUnequip]);
		}
		else	if (ItemToClearFromInput.ItemData.Asset.Get() != ItemToEquip.ItemData.Asset.Get() && InputToAssign.MatchesTagExact(InputToClear)) // Keep Input, Change Item Ability
		{
			GEngine->AddOnScreenDebugMessage(48953, 5.f, FColor::Green, FString::Printf(TEXT("Keep Input, Change Item Ability")));
			ULeyrAbilitySystemLibrary::UpdateItemAbilities(this, AbilitySystemComponent, ItemToClearFromInput.ItemData, InputToAssign, true);
			ULeyrAbilitySystemLibrary::UpdateItemAbilities(this, AbilitySystemComponent, ItemData, InputToAssign, false);
		}
		
		// 2. Assign
		Assign(InputToAssign, SlotToEquip, ItemToEquip);
	}
}

void UInventoryWidgetController::Assign(const FGameplayTag InputToAssign, const FGameplayTag SlotToEquip, FEquippedItem& ItemToEquip)
{
	// GEngine->AddOnScreenDebugMessage(48943, 5.f, FColor::White, FString::Printf(TEXT("Assign: [%s] - [%s]"), *InputToAssign.ToString(), *SlotToEquip.ToString()));
	OnInputAssigned.Broadcast(ItemToEquip.ItemData, InputToAssign);
	OnItemEquipped.Broadcast(SlotToEquip, ItemToEquip.ItemData);
	// const UItemData* Asset = ItemToEquip.ItemData.Asset.LoadSynchronous();
	// ItemToEquip.ActiveEffect = MakeAndApplyEffectToSelf(Asset, SlotToEquip, Asset->Modifiers);
	EquippedItems.Add(SlotToEquip, ItemToEquip);
}

void UInventoryWidgetController::Clear(const FGameplayTag InputToClear, const FGameplayTag SlotToUnequip)
{
	GEngine->AddOnScreenDebugMessage(48944, 5.f, FColor::White, FString::Printf(TEXT("Remove: [%s] - [%s]"), *InputToClear.ToString(), *SlotToUnequip.ToString()));
	OnInputRemoved.Broadcast(InputToClear);
	OnItemUnequipped.Broadcast(SlotToUnequip);
	// RemoveActiveGameplayEffect(SlotToUnequip);
	EquippedItems.Remove(SlotToUnequip);
}

bool UInventoryWidgetController::ClearEquipButtonByInputTag(const FGameplayTag InputTag, const UItemData* Asset)
{
	if(const FEquippedItem* EquippedItem = EquippedItems.Find(InputTag))
	{
		ClearInputTag(EquippedItem, InputTag);
		return Asset == EquippedItem->ItemData.Asset;
	}
	return false;
}

void UInventoryWidgetController::ClearInputTag(const FEquippedItem* EquippedItem, const FGameplayTag InputTag)
{
	ULeyrAbilitySystemLibrary::UpdateItemAbilities(this, AbilitySystemComponent, EquippedItem->ItemData, InputTag, true);
	ULeyrAbilitySystemLibrary::UpdateMonkAbilities(this, AbilitySystemComponent, InputTag, false);
	EquippedItems.Remove(InputTag);
	OnInputRemoved.Broadcast(InputTag);
	Unequip(InputTag);
	InventoryComponent->UpdateItemInputTag(EquippedItem->ItemData, FBaseGameplayTags::Get().InputTag_None);
}

bool UInventoryWidgetController::ReplaceInputTag(FInventoryItemData ItemData, const FGameplayTag InputTag)
{
	for (TTuple<FGameplayTag, FEquippedItem> EquippedItem : EquippedItems)
	{
		if (EquippedItem.Value.ItemData.Asset == ItemData.Asset)
		{
			ULeyrAbilitySystemLibrary::ReplaceAbilityInputTag(this, AbilitySystemComponent, ItemData, InputTag, EquippedItem.Key);
			ULeyrAbilitySystemLibrary::UpdateMonkAbilities(this, AbilitySystemComponent, EquippedItem.Key, false);
			
			EquippedItems.Remove(EquippedItem.Key);
			EquippedItems.Add(InputTag, FEquippedItem{ ItemData });
			OnInputRemoved.Broadcast(EquippedItem.Key);
			OnInputAssigned.Broadcast(ItemData, InputTag);
			
			Assign(ItemData, InputTag);
			Unequip(EquippedItem.Key);
			InventoryComponent->UpdateItemInputTag(ItemData, InputTag);
			return true;
		}
	}
	return false;
}

void UInventoryWidgetController::AssignInputTag(const FInventoryItemData& ItemData, const FGameplayTag InputTag)
{
	ULeyrAbilitySystemLibrary::UpdateMonkAbilities(this, AbilitySystemComponent, InputTag, true);
	ULeyrAbilitySystemLibrary::UpdateItemAbilities(this, AbilitySystemComponent, ItemData, InputTag, false);
	EquippedItems.Add(InputTag, FEquippedItem{ ItemData });
	OnInputAssigned.Broadcast(ItemData, InputTag);
	Assign(ItemData, InputTag);
	InventoryComponent->UpdateItemInputTag(ItemData, InputTag);
}

void UInventoryWidgetController::Assign(const FInventoryItemData& ItemData, FGameplayTag InputTag)
{
	const FInventoryItemData Item = ULeyrAbilitySystemLibrary::FindItemDataByRowName(this, ItemData.Name);
	const FBaseGameplayTags& GameplayTags = FBaseGameplayTags::Get();
	FGameplayTag Slot = Item.EquipmentSlot;

	if(Slot.MatchesTagExact(GameplayTags.Equipment_ActionSlot))
	{
		if(GameplayTags.EquipmentSlotToInputTags.Contains(InputTag))
		{
			Slot = GameplayTags.EquipmentSlotToInputTags[InputTag];
		}
	}
	
	EquippedItems.Add(Slot, FEquippedItem{ Item });
	OnItemEquipped.Broadcast(Slot, Item);
	
	UItemData* ItemDataAsset = ItemData.Asset.LoadSynchronous();
	for (TSubclassOf<UGameplayEffect> Effect : ItemDataAsset->Effects)
	{
		ApplyExecuteEffectToSelf(Effect.GetDefaultObject(), ItemData.Asset.Get(), Slot);
	}
	MakeAndApplyEffectToSelf(ItemData.Asset.Get(), Slot, ItemDataAsset->Modifiers);
}

void UInventoryWidgetController::ClearEquipButtonByItemData(const FInventoryItemData& ItemData)
{
	for (TTuple<FGameplayTag, FEquippedItem> EquippedItem : EquippedItems)
	{
		if(EquippedItem.Value.ItemData.Asset == ItemData.Asset)
		{
			ClearInputTag(&EquippedItem.Value, EquippedItem.Key);
			return;
		}
	}
}

/*
 * Equipping
 */
void UInventoryWidgetController::EquipButtonPressed(FInventoryItemData ItemData)
{
	// OnEquipmentAssigned.Broadcast(ItemData);
	Equip(ItemData);
}

void UInventoryWidgetController::Equip(const FInventoryItemData& ItemData)
{
	const FInventoryItemData Item = ULeyrAbilitySystemLibrary::FindItemDataByRowName(this, ItemData.Name); // Why do I do this ????
	FGameplayTag Slot = Item.EquipmentSlot;
	
	for (TTuple<FGameplayTag, FEquippedItem> EquippedItem : EquippedItems)
	{
		if (Slot.MatchesTagExact(EquippedItem.Key))
		{
			if (EquippedItems.Contains(Slot) && ItemData.Asset == EquippedItem.Value.ItemData.Asset)
			{
				EquippedItems.Remove(Slot);
				OnItemUnequipped.Broadcast(Slot);	
				RemoveActiveGameplayEffect(Slot);
				return;
			}
		}
	}
	EquippedItems.Add(Slot, FEquippedItem{ Item });
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
	
	if(GameplayTags.EquipmentSlotToInputTags.Contains(InputTag))
	{
		Slot = GameplayTags.EquipmentSlotToInputTags[InputTag];
	}
	EquippedItems.Remove(Slot);
	OnItemUnequipped.Broadcast(Slot);
	GEngine->AddOnScreenDebugMessage(9789444, 5.f, FColor::Magenta, "UInventoryWidgetController::Unequip");
	RemoveActiveGameplayEffect(Slot);
}

void UInventoryWidgetController::HandleItemUpdated(EContainerType ContainerType, int32 SlotIndex, FInventoryItemData Item)
{
	HandleOnItemUpdated.Broadcast(ContainerType, SlotIndex, Item);
	
	const UItemData* Asset =  Item.Asset.LoadSynchronous();
	if (Item.Quantity == 0 && Asset && Asset->bRemoveStackIfEmpty)
	{
		ClearEquipButtonByItemData(Item);
	}
}

UItemData* UInventoryWidgetController::HasCompatibleItemCostInAmmunitionSlot(const FGameplayTag CostTag)
{
	if (const FEquippedItem* EquippedItem = EquippedItems.Find(FBaseGameplayTags::Get().Equipment_Ammunition))
	{
		if(UItemData* Asset = EquippedItem->ItemData.Asset.LoadSynchronous())
		{
			return Asset->CostTag.MatchesTagExact(CostTag) ? Asset : nullptr;
		}
	}
	return nullptr;
}

/*
 * Container
*/
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

/*
 * Gameplay Effects
 */
FActiveGameplayEffectHandle UInventoryWidgetController::ApplyExecuteEffectToSelf(UGameplayEffect* EffectToApply, const UObject* SourceObject, const FGameplayTag EquipmentSlot, const int32 Level)
{
	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(SourceObject);
	
	UTargetTagsGameplayEffectComponent& AssetTagsComponent = EffectToApply->FindOrAddComponent<UTargetTagsGameplayEffectComponent>();
	FInheritedTagContainer InheritedTagContainer;
	InheritedTagContainer.Added.AddTag(EquipmentSlot);
	AssetTagsComponent.SetAndApplyTargetTagChanges(InheritedTagContainer);

	return AbilitySystemComponent->ApplyGameplayEffectToSelf(EffectToApply, Level, EffectContext);
}

FActiveGameplayEffectHandle UInventoryWidgetController::MakeAndApplyEffectToSelf(const UObject* SourceObject, const FGameplayTag& EquipmentSlot, TArray<FGameplayModifierInfo> Modifiers, const int32 Level)
{
	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(SourceObject);

	FString TagName = FString::Printf(TEXT("%s"), *EquipmentSlot.ToString());
	UGameplayEffect* Effect = NewObject<UGameplayEffect>(GetTransientPackage(), FName(TagName));

	Effect->DurationPolicy = EGameplayEffectDurationType::Infinite;
	
	UTargetTagsGameplayEffectComponent& AssetTagsComponent = Effect->FindOrAddComponent<UTargetTagsGameplayEffectComponent>();
	FInheritedTagContainer InheritedTagContainer;
	InheritedTagContainer.Added.AddTag(EquipmentSlot);
	AssetTagsComponent.SetAndApplyTargetTagChanges(InheritedTagContainer);

	for (int i = 0; i < Modifiers.Num(); ++i)
	{
		Effect->Modifiers.Add(Modifiers[i]);
	}

	if (const FGameplayEffectSpec* MutableSpec = new FGameplayEffectSpec(Effect, EffectContext, Level))
	{
		return AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*MutableSpec, AbilitySystemComponent);
	}
	return FActiveGameplayEffectHandle();
}

void UInventoryWidgetController::RemoveActiveGameplayEffect(FGameplayTag EquipmentSlot)
{
	if(EquippedItems.IsEmpty()) return;

	if (FEquippedItem* ItemToRemove = EquippedItems.Find(EquipmentSlot))
	{
		if(ItemToRemove->ActiveEffect.IsValid())
		{
			AbilitySystemComponent->RemoveActiveGameplayEffect(ItemToRemove->ActiveEffect);
		}
		
	}
	// FGameplayEffectQuery Query;
	// Query.EffectTagQuery.
	// AbilitySystemComponent->RemoveActiveEffects()
}