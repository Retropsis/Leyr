// @ Retropsis 2024-2025.

#include "UI/Controller/InventoryWidgetController.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"
#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "AbilitySystem/LeyrAbilitySystemLibrary.h"
#include "AbilitySystem/Data/LevelUpInfo.h"
#include "Game/BaseGameplayTags.h"
#include "Inventory/InventoryComponent.h"
#include "Player/PlayerCharacterState.h"

void UInventoryWidgetController::BindCallbacksToDependencies()
{
	checkf(InventoryComponent, TEXT("InventoryComponent is NULL, please check [%s]"), *GetName());
	InventoryComponent->OnItemUpdated.AddDynamic(this, &UInventoryWidgetController::HandleItemUpdated);
	
	GetBasePS()->OnXPChangedDelegate.AddUObject(this, &UInventoryWidgetController::OnXPChanged);
	GetBasePS()->OnLevelChangedDelegate.AddLambda(
		[this](int32 NewLevel)
		{
			OnPlayerLevelChanged.Broadcast(NewLevel);
		}
	);
	
	UpdateEquipmentEffect();
	UpdateMonkAbility();
}

/*
 * Assigning
 */
void UInventoryWidgetController::AssignButtonPressed(const FInventoryItemData ItemData, const FGameplayTag InputTag)
{
	if(ItemData.Asset.Get() == nullptr) return;
	const FBaseGameplayTags& GameplayTags = FBaseGameplayTags::Get();
	if(!ItemData.EquipmentSlot.MatchesTagExact(GameplayTags.Equipment_ActionSlot)) return;
	
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
		SlotToUnequip = SlotToEquip;
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

	PreviouslyEquippedItems = EquippedItems;

	// 1. Clear
	if(bShouldClear)
	{
		Clear(InputToClear, SlotToUnequip);
		if(ItemToClearFromInput.ItemData.Asset.Get() == ItemData.Asset.Get())
		{
			UpdateEquipmentEffect();
			UpdateItemAbilities();
			UpdateMonkAbility();
			return;
		}
	}
	
	// 2. Assign
	Assign(InputToAssign, SlotToEquip, ItemToEquip);
	
	UpdateEquipmentEffect();
	UpdateItemAbilities();
	UpdateMonkAbility();
}

void UInventoryWidgetController::Assign(const FGameplayTag InputToAssign, const FGameplayTag SlotToEquip, FEquippedItem& ItemToEquip)
{
	OnInputAssigned.Broadcast(ItemToEquip.ItemData, InputToAssign);
	OnItemEquipped.Broadcast(SlotToEquip, ItemToEquip.ItemData);

	if(const UItemData* Asset = ItemToEquip.ItemData.Asset.LoadSynchronous())
	{
		ItemToEquip.Modifiers = Asset->Modifiers;
		ItemToEquip.Abilities = Asset->Abilities;
	}
	EquippedItems.Add(SlotToEquip, ItemToEquip);
}

void UInventoryWidgetController::Clear(const FGameplayTag InputToClear, const FGameplayTag SlotToUnequip)
{
	OnInputRemoved.Broadcast(InputToClear);
	OnItemUnequipped.Broadcast(SlotToUnequip, EquippedItems[SlotToUnequip].ItemData.Asset);
	EquippedItems.Remove(SlotToUnequip);
}

/*
 * Equipping
 */
void UInventoryWidgetController::EquipButtonPressed(FInventoryItemData ItemData)
{
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
			if (EquippedItems.Contains(Slot))
			{
				OnItemUnequipped.Broadcast(Slot, EquippedItem.Value.ItemData.Asset);	
				EquippedItems.Remove(Slot);
				UpdateEquipmentEffect();
				
				if(ItemData.Asset.Get() == EquippedItem.Value.ItemData.Asset.Get()) return;
				
				break;
			}
		}
	}
	
	FEquippedItem ItemToEquip{ItemData };	
	ItemToEquip.Modifiers = Asset->Modifiers;
	
	OnItemEquipped.Broadcast(Slot, ItemData);
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
void UInventoryWidgetController::LootButtonPressed(int32 SourceSlotIndex) const
{
	if(!bContainerIsOpen) return;
	if(UInventoryComponent* ContainerComponent = ULeyrAbilitySystemLibrary::GetContainerComponent(this))
	{
		ContainerComponent->TransferItemToEmptySlot(InventoryComponent, SourceSlotIndex);
	}
}

void UInventoryWidgetController::LootAllButtonPressed() const
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
void UInventoryWidgetController::UpdateEquipmentEffect()
{
	if(ActiveEquipmentEffectHandle.IsValid()) AbilitySystemComponent->RemoveActiveGameplayEffect(ActiveEquipmentEffectHandle);
	
	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	FString SourceObjectName = FString::Printf(TEXT("GE_EquipmentEffects"));
	UGameplayEffect* Effect = NewObject<UGameplayEffect>(this, FName(SourceObjectName));

	Effect->DurationPolicy = EGameplayEffectDurationType::Infinite;
	
	// UTargetTagsGameplayEffectComponent& AssetTagsComponent = Effect->FindOrAddComponent<UTargetTagsGameplayEffectComponent>();
	// FInheritedTagContainer InheritedTagContainer;
	// InheritedTagContainer.Added.AddTag(EquipmentSlot);
	// AssetTagsComponent.SetAndApplyTargetTagChanges(InheritedTagContainer);

	for (const TTuple<FGameplayTag, FEquippedItem> EquippedItem : EquippedItems)
	{
		if (EquippedItem.Key.MatchesTag(FBaseGameplayTags::Get().Equipment_ActionSlot)) continue;
		Effect->Modifiers.Append(EquippedItem.Value.Modifiers);
	}

	if (FGameplayEffectSpec* MutableSpec = new FGameplayEffectSpec(Effect, EffectContext, 1.f))
	{
		ActiveEquipmentEffectHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*MutableSpec, AbilitySystemComponent);
	}
}

void UInventoryWidgetController::UpdateMonkAbility()
{	
	const FBaseGameplayTags& GameplayTags = FBaseGameplayTags::Get();
	FGameplayTagContainer ActionSlots;
	ActionSlots.AddTag(GameplayTags.InputTag_LMB);
	ActionSlots.AddTag(GameplayTags.InputTag_RMB);
	ActionSlots.AddTag(GameplayTags.InputTag_1);

	const FGameplayTagQuery ActionSlotQuery = ActionSlotQuery.BuildQuery(
		FGameplayTagQueryExpression()
		.AnyTagsMatch()
		.AddTag(GameplayTags.Equipment_ActionSlot_1)
		.AddTag(GameplayTags.Equipment_ActionSlot_2)
		.AddTag(GameplayTags.Equipment_ActionSlot_3)
	);
	for (TTuple<FGameplayTag, FEquippedItem> EquippedItem : EquippedItems)
	{
		if(EquippedItem.Key.GetSingleTagContainer().MatchesQuery(ActionSlotQuery))
		{
			ActionSlots.RemoveTag(GameplayTags.EquipmentSlotToInputTags[EquippedItem.Key]);
		}
	}
	ULeyrAbilitySystemLibrary::UpdateMonkAbilities(this, AbilitySystemComponent, ActionSlots);
}

void UInventoryWidgetController::UpdateItemAbilities()
{
	const FBaseGameplayTags& GameplayTags = FBaseGameplayTags::Get();
	for (TTuple<FGameplayTag, FEquippedItem> EquippedItem : PreviouslyEquippedItems)
	{
		if(EquippedItem.Key.MatchesTag(GameplayTags.Equipment_ActionSlot))
		{
			// Was it replaced ?
			if (FEquippedItem* FoundItem = EquippedItems.Find(EquippedItem.Key))
			{
				if(EquippedItem.Value.ItemData.Asset.Get() != FoundItem->ItemData.Asset.Get())
				{
					ULeyrAbilitySystemLibrary::UpdateAbilities(this, AbilitySystemComponent, FoundItem->ItemData.Asset.Get(), GameplayTags.EquipmentSlotToInputTags[EquippedItem.Key], FoundItem->Abilities);
				}
			}
			// Did it move ?
			bool bItemFound = false;
			for (TTuple<FGameplayTag, FEquippedItem> NewlyEquippedItem : EquippedItems)
			{
				if(NewlyEquippedItem.Key.MatchesTag(GameplayTags.Equipment_ActionSlot))
				{
					if(EquippedItem.Value.ItemData.Asset.Get() == NewlyEquippedItem.Value.ItemData.Asset.Get())
					{
						ULeyrAbilitySystemLibrary::UpdateAbilities(
							this, AbilitySystemComponent,
							NewlyEquippedItem.Value.ItemData.Asset.Get(),
							GameplayTags.EquipmentSlotToInputTags[NewlyEquippedItem.Key],
							NewlyEquippedItem.Value.Abilities);
						bItemFound = true;
						break;
					}
				}
			}
			if(bItemFound) continue;
			
			// Clear
			if(UBaseAbilitySystemComponent* BaseASC = Cast<UBaseAbilitySystemComponent>(AbilitySystemComponent))
			{
				for (FGameplayTag AbilityTag : EquippedItem.Value.Abilities)
				{
					FGameplayAbilitySpec* AbilitySpec = BaseASC->GetSpecFromAbilityTag(AbilityTag);
					AbilitySystemComponent->ClearAbility(AbilitySpec->Handle);
					AbilitySystemComponent->MarkAbilitySpecDirty(*AbilitySpec);
				}
			}
		}
	}

	for (TTuple<FGameplayTag, FEquippedItem> EquippedItem : EquippedItems)
	{
		if(EquippedItem.Key.MatchesTag(GameplayTags.Equipment_ActionSlot))
		{
			if (!PreviouslyEquippedItems.Contains(EquippedItem.Key))
			{
				// Add
				ULeyrAbilitySystemLibrary::UpdateAbilities(this, AbilitySystemComponent, EquippedItem.Value.ItemData.Asset.Get(), GameplayTags.EquipmentSlotToInputTags[EquippedItem.Key], EquippedItem.Value.Abilities);
			}
		}
	}
}

void UInventoryWidgetController::OnXPChanged(int32 NewXP)
{
	const ULevelUpInfo* LevelUpInfo = GetBasePS()->LevelUpInfo;
	checkf(LevelUpInfo, TEXT("Unabled to find LevelUpInfo. Please fill out BasePlayerState Blueprint"));

	const int32 Level = LevelUpInfo->FindLevelForXP(NewXP);
	const int32 MaxLevel = LevelUpInfo->LevelUpInformation.Num();

	if (Level <= MaxLevel && Level > 0)
	{
		const int32 LevelUpRequirement = LevelUpInfo->LevelUpInformation[Level].LevelUpRequirement;
		const int32 PreviousLevelUpRequirement = LevelUpInfo->LevelUpInformation[Level - 1].LevelUpRequirement;

		const int32 DeltaLevelRequirement = LevelUpRequirement - PreviousLevelUpRequirement;
		const int32 XPForThisLevel = NewXP - PreviousLevelUpRequirement;

		const float XPBarPercent = static_cast<float>(XPForThisLevel) / static_cast<float>(DeltaLevelRequirement);

		OnXPPercentChanged.Broadcast(XPBarPercent);
		OnXPValueUpdated.Broadcast(XPForThisLevel);
	}
}
