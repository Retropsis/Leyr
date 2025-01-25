// @ Retropsis 2024-2025.

#include "UI/Controller/InventoryWidgetController.h"
#include "AbilitySystemBlueprintLibrary.h"
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

	FGameplayEffectSpecHandle EquipmentSpecHandle = AbilitySystemComponent->MakeOutgoingSpec(EquipmentEffectClass, 1.f, AbilitySystemComponent->MakeEffectContext());
	
	ActiveEquipmentEffectHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*EquipmentSpecHandle.Data.Get(), AbilitySystemComponent);
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
			// SlotToUnequip = EquippedItem.Key;
			bShouldClear = true;
		}

		// ITEM ALREADY ASSIGNED
		for (TTuple<FGameplayTag, FEquippedItem> EquippedItem : EquippedItems)
		{
			if (EquippedItem.Value.ItemData.Asset.Get() == ItemData.Asset.Get())
			{
				SlotToUnequip = EquippedItem.Key;
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
			ULeyrAbilitySystemLibrary::UpdateItemAbilities(this, AbilitySystemComponent, ItemToClearFromInput.ItemData, GameplayTags.EquipmentSlotToInputTags[SlotToUnequip], true);
			ULeyrAbilitySystemLibrary::ReplaceAbilityInputTag(this, AbilitySystemComponent, ItemData, InputToAssign, GameplayTags.EquipmentSlotToInputTags[SlotToUnequip]);
			ULeyrAbilitySystemLibrary::UpdateMonkAbilities(this, AbilitySystemComponent, GameplayTags.EquipmentSlotToInputTags[SlotToUnequip], false);
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

	// for (TSubclassOf<UGameplayEffect> Effect : Asset->Effects)
	// {
	// 	FGameplayEffectContextHandle ContextHandle = AbilitySystemComponent->MakeEffectContext();
	// 	FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(Effect, 1.f, ContextHandle);
	// 	const_cast<UGameplayEffect*>(SpecHandle.Data.Get()->Def.Get())->Modifiers = Asset->Modifiers;
	// 	ItemToEquip.ActiveEffect = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	// }

	if(const UItemData* Asset = ItemToEquip.ItemData.Asset.LoadSynchronous())
	{
		ItemToEquip.Modifiers = Asset->Modifiers;
	}
	EquippedItems.Add(SlotToEquip, ItemToEquip);
	UpdateEquipmentEffect();
}

void UInventoryWidgetController::Clear(const FGameplayTag InputToClear, const FGameplayTag SlotToUnequip)
{
	GEngine->AddOnScreenDebugMessage(48944, 5.f, FColor::White, FString::Printf(TEXT("Remove: [%s] - [%s]"), *InputToClear.ToString(), *SlotToUnequip.ToString()));
	OnInputRemoved.Broadcast(InputToClear);
	OnItemUnequipped.Broadcast(SlotToUnequip);
	// RemoveActiveGameplayEffect(SlotToUnequip);
	EquippedItems.Remove(SlotToUnequip);
	UpdateEquipmentEffect();
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
	UItemData* Asset = ItemData.Asset.LoadSynchronous();
	FGameplayTag Slot = ItemData.EquipmentSlot;
	
	if(Asset == nullptr || Slot.MatchesTagExact(FBaseGameplayTags::Get().Equipment_ActionSlot)) return;
	
	for (TTuple<FGameplayTag, FEquippedItem> EquippedItem : EquippedItems)
	{
		if (Slot.MatchesTagExact(EquippedItem.Key))
		{
			if (EquippedItems.Contains(Slot) && ItemData.Asset.Get() == EquippedItem.Value.ItemData.Asset.Get())
			{
				RemoveActiveGameplayEffect(Slot);
				OnItemUnequipped.Broadcast(Slot);	
				EquippedItems.Remove(Slot);
				UpdateEquipmentEffect();
				return;
			}
		}
	}
	
	FEquippedItem ItemToEquip{ItemData };
	// for (const TSubclassOf<UGameplayEffect> Effect : Asset->Effects)
	// {
	// 	const FGameplayEffectContextHandle ContextHandle = AbilitySystemComponent->MakeEffectContext();
	// 	FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(Effect, 1.f, ContextHandle);
	// 	const_cast<UGameplayEffect*>(SpecHandle.Data.Get()->Def.Get())->Modifiers = Asset->Modifiers;
	// 	ItemToEquip.ActiveEffect = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	// }
	
	// for (TSubclassOf<UGameplayEffect> Effect : Asset->Effects)
	// {
	// 	ApplyExecuteEffectToSelf(Effect.GetDefaultObject(), ItemData.Asset.Get(), Slot);
	// }
	// MakeAndApplyEffectToSelf(ItemData.Asset.Get(), Slot, Asset->Modifiers);
	
	OnItemEquipped.Broadcast(Slot, ItemData);
	ItemToEquip.Modifiers = Asset->Modifiers;
	EquippedItems.Add(Slot, ItemToEquip);
	UpdateEquipmentEffect();
}

void UInventoryWidgetController::HandleItemUpdated(EContainerType ContainerType, int32 SlotIndex, FInventoryItemData Item)
{
	HandleOnItemUpdated.Broadcast(ContainerType, SlotIndex, Item);
	
	const UItemData* Asset =  Item.Asset.LoadSynchronous();
	if (Item.Quantity == 0 && Asset && Asset->bRemoveStackIfEmpty)
	{
		for (TTuple<FGameplayTag, FEquippedItem> EquippedItem : EquippedItems)
		{
			if(EquippedItem.Value.ItemData.Asset.Get() == Item.Asset.Get())
			{
				Clear(FBaseGameplayTags::Get().EquipmentSlotToInputTags[EquippedItem.Key], EquippedItem.Key);
				return;
			}
		}
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

	FString SourceObjectName = FString::Printf(TEXT("%s_AddedEffects"), *SourceObject->GetName());
	UGameplayEffect* Effect = NewObject<UGameplayEffect>(this, FName(SourceObjectName));

	Effect->DurationPolicy = EGameplayEffectDurationType::Infinite;
	
	UTargetTagsGameplayEffectComponent& AssetTagsComponent = Effect->FindOrAddComponent<UTargetTagsGameplayEffectComponent>();
	FInheritedTagContainer InheritedTagContainer;
	InheritedTagContainer.Added.AddTag(EquipmentSlot);
	AssetTagsComponent.SetAndApplyTargetTagChanges(InheritedTagContainer);

	for (int i = 0; i < Modifiers.Num(); ++i)
	{
		Effect->Modifiers.Add(Modifiers[i]);
	}

	if (FGameplayEffectSpec* MutableSpec = new FGameplayEffectSpec(Effect, EffectContext, Level))
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
		GEngine->AddOnScreenDebugMessage(35777, 5.f, FColor::Magenta, FString::Printf(TEXT("Removing: [%s]"), *ItemToRemove->ActiveEffect.ToString()));
		if(ItemToRemove->ActiveEffect.IsValid())
		{
			AbilitySystemComponent->RemoveActiveGameplayEffect(ItemToRemove->ActiveEffect);
		}
	}
	// FGameplayEffectQuery Query;
	// Query.EffectTagQuery.
	// AbilitySystemComponent->RemoveActiveEffects()
}

void UInventoryWidgetController::UpdateEquipmentEffect()
{
	if(ActiveEquipmentEffectHandle.IsValid()) AbilitySystemComponent->RemoveActiveGameplayEffect(ActiveEquipmentEffectHandle);
	
	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	FString SourceObjectName = FString::Printf(TEXT("%s"), *EquipmentEffectClass->GetName());
	UGameplayEffect* Effect = NewObject<UGameplayEffect>(this, FName(SourceObjectName));

	Effect->DurationPolicy = EGameplayEffectDurationType::Infinite;
	
	// UTargetTagsGameplayEffectComponent& AssetTagsComponent = Effect->FindOrAddComponent<UTargetTagsGameplayEffectComponent>();
	// FInheritedTagContainer InheritedTagContainer;
	// InheritedTagContainer.Added.AddTag(EquipmentSlot);
	// AssetTagsComponent.SetAndApplyTargetTagChanges(InheritedTagContainer);

	for (TTuple<FGameplayTag, FEquippedItem> EquippedItem : EquippedItems)
	{
		// const UItemData* Asset = EquippedItem.Value.ItemData.Asset.LoadSynchronous();
		// if(Asset == nullptr) continue;
		for (int i = 0; i < EquippedItem.Value.Modifiers.Num(); ++i)
		{
			Effect->Modifiers.Add(EquippedItem.Value.Modifiers[i]);
		}
	}

	if (FGameplayEffectSpec* MutableSpec = new FGameplayEffectSpec(Effect, EffectContext, 1.f))
	{
		ActiveEquipmentEffectHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*MutableSpec, AbilitySystemComponent);
	}
}
