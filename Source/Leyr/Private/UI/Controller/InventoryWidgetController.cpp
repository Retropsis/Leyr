// @ Retropsis 2024-2025.

#include "UI/Controller/InventoryWidgetController.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"
#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "AbilitySystem/LeyrAbilitySystemLibrary.h"
#include "AbilitySystem/Data/LevelUpInfo.h"
#include "Data/AbilitySet.h"
#include "Data/InventoryCostData.h"
#include "Engine/AssetManager.h"
#include "Game/BaseGameplayTags.h"
#include "Game/LeyrGameInstance.h"
#include "Game/LeyrGameMode.h"
#include "Game/LoadMenuSaveGame.h"
#include "Inventory/InventoryComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Player/PlayerCharacterState.h"

void UInventoryWidgetController::BroadcastInitialValues()
{
	OnPlayerLevelChanged.Broadcast(GetBasePS()->GetCharacterLevel());
}

void UInventoryWidgetController::BindCallbacksToDependencies()
{
	checkf(InventoryComponent, TEXT("InventoryComponent is NULL, please check [%s]"), *GetName());
	
	InventoryComponent->OnItemQuantityUpdated.AddLambda([this](const FInventoryItemData& ItemData)
		{
			for (TTuple<FGameplayTag, FEquippedItem> EquippedItem : EquippedItems)
			{
				if (EquippedItem.Value.ItemData.Asset.Get() == ItemData.Asset.Get())
				{
					OnEquipmentSlotQuantityUpdated.Broadcast(EquippedItem.Key, ItemData);
				}
			}
		}
	);
	// InventoryComponent->OnItemAdded.AddLambda([this](const FInventoryItemData& ItemData)
	// 	{
	// 		const FBaseGameplayTags& GameplayTags = FBaseGameplayTags::Get();
	// 		BaseASC->AbilityCostCommitted.Broadcast(GameplayTags.Abilities_None, GameplayTags.Cost_None, -1);
	// 		UpdateAmmunitionCounter(GameplayTags.Equipment_Ammunition, )
	// 	}
	// );
	
	GetBasePS()->OnXPChangedDelegate.AddUObject(this, &UInventoryWidgetController::OnXPChanged);
	GetBasePS()->OnLevelChangedDelegate.AddLambda(
		[this](int32 NewLevel, bool bLevelUp)
		{
			OnPlayerLevelChanged.Broadcast(NewLevel);
		}
	);
	
	const ALeyrGameMode* LeyrGameMode = Cast<ALeyrGameMode>(UGameplayStatics::GetGameMode(this));
	const ULeyrGameInstance* LeyrGameInstance = Cast<ULeyrGameInstance>(LeyrGameMode->GetGameInstance());
	if (const ULoadMenuSaveGame* SaveGame = LeyrGameMode->GetSaveSlotData(LeyrGameInstance->LoadSlotName, LeyrGameInstance->LoadSlotIndex))
	{
		EquippedItems = SaveGame->SavedEquippedItems;
	}
	
	const FBaseGameplayTags& GameplayTags = FBaseGameplayTags::Get();
	FGameplayTagContainer MonkInputTags;
	FEquippedItem MonkAbility = FEquippedItem{};
	MonkSet->GiveToAbilitySystem(AbilitySystemComponent, &MonkAbility.OutGrantedHandles, FGameplayTag());
	
	EquippedItems.Add(GameplayTags.Equipment_ActionSlot_0, MonkAbility);
	EquippedItems.FindOrAdd(GameplayTags.Equipment_ActionSlot_MainHand, FEquippedItem{});
	EquippedItems.FindOrAdd(GameplayTags.Equipment_ActionSlot_OffHand, FEquippedItem{});
	EquippedItems.FindOrAdd(GameplayTags.Equipment_ActionSlot_Range, FEquippedItem{});
	for (TTuple<FGameplayTag, FEquippedItem> ActionSlot : GetActionSlots())
	{
		if (ActionSlot.Key.MatchesTagExact(GameplayTags.Equipment_ActionSlot_0)) continue;;
		
		FGameplayTag InputTag = GameplayTags.EquipmentSlotToInputTags[ActionSlot.Key];
		if (ActionSlot.Value.ItemData.Asset.ToSoftObjectPath().IsValid())
		{
			AsyncUpdateAbilities(ActionSlot.Value, InputTag);
		}
		else
		{
			MonkInputTags.AddTag(InputTag);
			GetBaseASC()->ClientEquipAbility(GameplayTags.Abilities_Weapon_Monk, GameplayTags.Abilities_Status_Equipped, InputTag, FGameplayTag());
		}
	}
	MonkAbility.OutGrantedHandles.UpdateInputTags(AbilitySystemComponent, MonkInputTags);

	// Init Equipment
	for (TTuple<FGameplayTag, FEquippedItem> EquipmentSlot : GetEquipmentSlots())
	{
		OnItemEquipped.Broadcast(EquipmentSlot.Key, EquipmentSlot.Value.ItemData);
	}
	UpdateEquipmentEffect();
}

/*
 * Assigning
*/
void UInventoryWidgetController::AssignButtonPressedLocked(FInventoryItemData ItemData)
{
	if(ItemData.Asset.Get() == nullptr) return;
	
	const FBaseGameplayTags& GameplayTags = FBaseGameplayTags::Get();
	const FGameplayTag InputTag = GameplayTags.EquipmentSlotToInputTags[ItemData.Asset.Get()->EquipmentSlot];
	AssignButtonPressed(ItemData, InputTag);
}

void UInventoryWidgetController::AssignButtonPressed(const FInventoryItemData ItemData, const FGameplayTag InputTag)
{
	if(ItemData.Asset.Get() == nullptr) return;
	
	const FBaseGameplayTags& GameplayTags = FBaseGameplayTags::Get();
	FGameplayTag SlotTag = GameplayTags.EquipmentSlotToInputTags[InputTag];

	// First check if Item is already assigned and remove it
	for (TTuple<FGameplayTag, FEquippedItem> EquippedItem : GetActionSlots())
	{
		// Item was found
		GEngine->AddOnScreenDebugMessage(-1, 90.f, FColor::Emerald, FString::Printf(TEXT("Item to Add: %s"), *ItemData.Asset.ToSoftObjectPath().ToString()));
		GEngine->AddOnScreenDebugMessage(-1, 90.f, FColor::Red, FString::Printf(TEXT("Item to Compare: %s"), *EquippedItem.Value.ItemData.Asset.ToSoftObjectPath().ToString()));
		if (ItemData.Asset.LoadSynchronous() == EquippedItem.Value.ItemData.Asset.LoadSynchronous())
		{
			// Remove it
			EquippedItem.Value.OutGrantedHandles.TakeFromAbilitySystem(AbilitySystemComponent);
			EquippedItems.Add(EquippedItem.Key, FEquippedItem{});
			OnInputRemoved.Broadcast(InputTag);
			OnItemUnequipped.Broadcast(EquippedItem.Key, ItemData.Asset);
			
			EquippedItems[GameplayTags.Equipment_ActionSlot_0].OutGrantedHandles.UpdateInputTags(AbilitySystemComponent, GameplayTags.EquipmentSlotToInputTags[EquippedItem.Key].GetSingleTagContainer(), ETagOperation::Add);
			GetBaseASC()->ClientEquipAbility(GameplayTags.Abilities_Weapon_Monk, GameplayTags.Abilities_Status_Equipped, GameplayTags.EquipmentSlotToInputTags[EquippedItem.Key], FGameplayTag());
		}
		// Slot that was required to be assigned
		if (EquippedItem.Key.MatchesTagExact(SlotTag))
		{
			// Some Item is already here, remove it
			EquippedItem.Value.OutGrantedHandles.TakeFromAbilitySystem(AbilitySystemComponent);
			EquippedItems.Add(EquippedItem.Key, FEquippedItem{});
			OnInputRemoved.Broadcast(InputTag);
			OnItemUnequipped.Broadcast(EquippedItem.Key, ItemData.Asset);
			
			// Add the new one if not the same
			if (ItemData.Asset.Get() != EquippedItem.Value.ItemData.Asset.Get())
			{
				EquippedItems.Add(EquippedItem.Key, FEquippedItem{ ItemData });
				AsyncUpdateAbilities(EquippedItems[EquippedItem.Key], InputTag);
				EquippedItems[GameplayTags.Equipment_ActionSlot_0].OutGrantedHandles.UpdateInputTags(AbilitySystemComponent, InputTag.GetSingleTagContainer(), ETagOperation::Remove);
			}
			else
			{
				// Update Monk Ability Tag
				EquippedItems[GameplayTags.Equipment_ActionSlot_0].OutGrantedHandles.UpdateInputTags(AbilitySystemComponent, InputTag.GetSingleTagContainer(), ETagOperation::Add);
				GetBaseASC()->ClientEquipAbility(GameplayTags.Abilities_Weapon_Monk, GameplayTags.Abilities_Status_Equipped, InputTag, FGameplayTag());
			}
		}
	}
}

void UInventoryWidgetController::AsyncUpdateAbilities(FEquippedItem& ItemToEquip, FGameplayTag InputTag)
{
	TSoftObjectPtr<UItemData> AssetToLoad = ItemToEquip.ItemData.Asset;
	UAssetManager::GetStreamableManager().RequestAsyncLoad(AssetToLoad.ToSoftObjectPath(), [this, AssetToLoad, InputTag, ItemToEquip] () {
		UItemData* LoadedAsset = AssetToLoad.Get();
		if (IsValid(LoadedAsset))
		{
			if (IsValid(LoadedAsset->AbilitySet))
			{
				const FBaseGameplayTags& GameplayTags = FBaseGameplayTags::Get();
				FEquippedItem NewlyEquippedItem = ItemToEquip;
				LoadedAsset->AbilitySet->GiveToAbilitySystem(AbilitySystemComponent, &NewlyEquippedItem.OutGrantedHandles, InputTag, 1.f, LoadedAsset);
				EquippedItems.Add(GameplayTags.EquipmentSlotToInputTags[InputTag], NewlyEquippedItem);
				GetBaseASC()->ClientEquipAbility(LoadedAsset->AbilitySet->AbilityTag, GameplayTags.Abilities_Status_Equipped, InputTag, FGameplayTag());
				OnInputAssigned.Broadcast(ItemToEquip.ItemData, InputTag, !bIsLoading);
				OnItemEquipped.Broadcast(GameplayTags.EquipmentSlotToInputTags[InputTag] , ItemToEquip.ItemData);
			}
		}
	}, FStreamableManager::AsyncLoadHighPriority);
}

/*
 * Equipping
 */
void UInventoryWidgetController::EquipButtonPressed(FInventoryItemData ItemData)
{
	if(ItemData.Asset.Get() == nullptr) return;
	
	const FBaseGameplayTags& GameplayTags = FBaseGameplayTags::Get();
	if (ItemData.Asset.Get()->EquipmentSlot.MatchesTag(GameplayTags.Equipment_ActionSlot))
	{
		const FGameplayTag InputTag = GameplayTags.EquipmentSlotToInputTags[ItemData.Asset.Get()->EquipmentSlot];
		AssignButtonPressed(ItemData, InputTag);
	}
	else
	{
		Equip(ItemData);
	}
}

void UInventoryWidgetController::Equip(const FInventoryItemData& ItemData)
{
	TSoftObjectPtr<UItemData> AssetToLoad = ItemData.Asset;
	UAssetManager::GetStreamableManager().RequestAsyncLoad(AssetToLoad.ToSoftObjectPath(), [this, AssetToLoad, ItemData] () {
		UItemData* LoadedAsset = AssetToLoad.Get();
		if (IsValid(LoadedAsset))
		{
			FGameplayTag Slot = ItemData.Asset.Get()->EquipmentSlot;
			for (TTuple<FGameplayTag, FEquippedItem>& EquippedItem : GetEquipmentSlots())
			{
				// Remove the item already here
				if (Slot.MatchesTagExact(EquippedItem.Key))
				{
					if (const FEquippedItem* FoundItem = EquippedItems.Find(Slot))
					{
						OnItemUnequipped.Broadcast(Slot, FoundItem->ItemData.Asset);	
						EquippedItems.Add(Slot, FEquippedItem{});
						UpdateEquipmentEffect();
						
						if(ItemData.Asset.Get() == EquippedItem.Value.ItemData.Asset.Get()) return;
						break;
					}
				}
			}
			
			FEquippedItem ItemToEquip{ItemData };	
			ItemToEquip.Modifiers = LoadedAsset->Modifiers;
			OnItemEquipped.Broadcast(Slot, ItemData);
			EquippedItems.Add(Slot, ItemToEquip);
			UpdateEquipmentEffect();
		}
	}, FStreamableManager::AsyncLoadHighPriority);
}

UItemData* UInventoryWidgetController::HasCompatibleItemCostInAmmunitionSlot(const FGameplayTag CostTag)
{
	const FBaseGameplayTags& GameplayTags = FBaseGameplayTags::Get();
	for (TTuple<FGameplayTag, FEquippedItem> EquippedItem : EquippedItems)
	{
		if(EquippedItem.Key.MatchesTag(GameplayTags.Equipment_Ammunition))
		{
			if(UItemData* Asset = EquippedItem.Value.ItemData.Asset.LoadSynchronous())
            {
            	return Asset->CostTag.MatchesTagExact(CostTag) ? Asset : nullptr;
            }
		}
	}
	return nullptr;
}

bool UInventoryWidgetController::HasCompatibleItemCostInInventory(const FGameplayTag CostTag, FInventoryItemData& OutItem) const
{
	if(const UInventoryCostData* InventoryCostData = ULeyrAbilitySystemLibrary::GetInventoryCostData(this))
	{		
		FInventoryCost InventoryCost = InventoryCostData->FindCostInfoForTag(CostTag);
		for (const UItemData* CompatibleItem : InventoryCost.CompatibleItems)
		{
			for (FInventoryItemData ItemData : InventoryComponent->Items)
			{
				if (ItemData.Asset.Get() == CompatibleItem)
				{
					OutItem = ItemData;
					return true;
				}
			}
		}
	}
	return false;
}

void UInventoryWidgetController::UpdateInventorySlots() const
{
	InventoryComponent->UpdateInventorySlots();
}

void UInventoryWidgetController::RequestUpdateInventorySlotsOnce()
{
	if (bRequestUpdateInventorySlots)
	{
		InventoryComponent->UpdateInventorySlots();
		BroadcastEquippedItems();
		bRequestUpdateInventorySlots = false;
		FTimerHandle InitTimer;
		GetWorld()->GetTimerManager().SetTimer(InitTimer, FTimerDelegate::CreateLambda([this] () { bIsLoading = false; }), 1.f, false);
	}
}

void UInventoryWidgetController::BroadcastEquippedItems()
{
	const FBaseGameplayTags& GameplayTags = FBaseGameplayTags::Get();
	for (TTuple<FGameplayTag, FEquippedItem> Item : GetActionSlots())
	{
		if (Item.Value.ItemData.Asset.ToSoftObjectPath().IsValid())
		{
			OnInputAssigned.Broadcast(Item.Value.ItemData, GameplayTags.EquipmentSlotToInputTags[Item.Key],  !bIsLoading);
		}
	}
	for (TTuple<FGameplayTag, FEquippedItem> Item : GetEquipmentSlots())
	{
		if (Item.Value.ItemData.Asset.ToSoftObjectPath().IsValid())
		{
			OnItemEquipped.Broadcast(Item.Key, Item.Value.ItemData);
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

	for (const TTuple<FGameplayTag, FEquippedItem> EquippedItem : GetEquipmentSlots())
	{
		Effect->Modifiers.Append(EquippedItem.Value.Modifiers);
	}

	if (FGameplayEffectSpec* MutableSpec = new FGameplayEffectSpec(Effect, EffectContext, 1.f))
	{
		ActiveEquipmentEffectHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*MutableSpec, AbilitySystemComponent);
	}
}

void UInventoryWidgetController::UpdateAmmunitionCounter(FGameplayTag Slot, FEquippedItem EquippedItem) const
{
	const FBaseGameplayTags& GameplayTags = FBaseGameplayTags::Get();
	if(UBaseAbilitySystemComponent* BaseASC = Cast<UBaseAbilitySystemComponent>(AbilitySystemComponent))
	{
		if(EquippedItem.ItemData.Asset.Get()->CostTag.IsValid())
		{
			FInventoryItemData CompatibleAmmunition;
			int32 Quantity = HasCompatibleItemCostInInventory(EquippedItem.ItemData.Asset.Get()->CostTag, CompatibleAmmunition) ? CompatibleAmmunition.Quantity : 0;
			BaseASC->AbilityCostCommitted.Broadcast(EquippedItem.ItemData.Asset.Get()->AbilitySet->AbilityTag, EquippedItem.ItemData.Asset.Get()->CostTag, Quantity);
		}
		else
		{
			BaseASC->AbilityCostCommitted.Broadcast(GameplayTags.Abilities_None, GameplayTags.Cost_None, -1);
		}
	}
}

TMap<FGameplayTag, FEquippedItem> UInventoryWidgetController::GetActionSlots()
{
	if (EquippedItems.Num() == 0) return TMap<FGameplayTag, FEquippedItem>();
	
	const FBaseGameplayTags& GameplayTags = FBaseGameplayTags::Get();
	
	TMap<FGameplayTag, FEquippedItem> ActionSlotItems;
	for (TTuple<FGameplayTag, FEquippedItem> ActionSlotItem : EquippedItems)
	{
		if (ActionSlotItem.Key.MatchesTagExact(GameplayTags.Equipment_ActionSlot_0)) continue;;
		if (ActionSlotItem.Key.MatchesTag(GameplayTags.Equipment_ActionSlot)) ActionSlotItems.Add(ActionSlotItem.Key, ActionSlotItem.Value);
	}
	return ActionSlotItems;
}

TMap<FGameplayTag, FEquippedItem> UInventoryWidgetController::GetEquipmentSlots()
{
	if (EquippedItems.Num() == 0) return TMap<FGameplayTag, FEquippedItem>();
	
	const FBaseGameplayTags& GameplayTags = FBaseGameplayTags::Get();
	
	TMap<FGameplayTag, FEquippedItem> EquipmentSlotItems;
	for (TTuple<FGameplayTag, FEquippedItem> EquipmentSlotItem : EquippedItems)
	{
		if (EquipmentSlotItem.Key.MatchesTag(GameplayTags.Equipment_ActionSlot)) continue;
		EquipmentSlotItems.Add(EquipmentSlotItem.Key, EquipmentSlotItem.Value);
	}
	return EquipmentSlotItems;
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
 * GETTERS
 */
TArray<FInventoryItemData> UInventoryWidgetController::GetFilteredItems(const FGameplayTagContainer& Filters)
{
	TArray<FInventoryItemData> FilteredItems;
	for (const FInventoryItemData& Item : InventoryComponent->Items)
	{
		if (Filters.HasTag(Item.EquipmentSlot))
		{
			FilteredItems.Add(Item);
		}
	}
	return FilteredItems;
}

void UInventoryWidgetController::UseItem(const FInventoryItemData& Item, int32& OutQuantity)
{
	if (AbilitySystemComponent && Item.Asset.Get()->AbilitySet)
	{
		FGameplayAbilitySpecHandle AbilitySpecHandle = Item.Asset.Get()->AbilitySet->ActivateMenuAbility(AbilitySystemComponent, Item.Asset.Get());
		
	}
}

void UInventoryWidgetController::GetDescription(FString Description)
{
	OnDescriptionUpdated.Broadcast(Description, FString());
}

/*
 * Experience Points
 */
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
