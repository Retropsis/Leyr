// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "ActiveGameplayEffectHandle.h"
#include "GameplayTagContainer.h"
#include "Data/AbilitySet.h"
#include "Inventory/InventoryComponent.h"
#include "UI/Controller/WidgetController.h"
#include "InventoryWidgetController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnInputAssignedSignature, FInventoryItemData, ItemData, FGameplayTag, InputTag);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInputRemovedSignature, FGameplayTag, InputTag);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnItemEquippedSignature, FGameplayTag, SlotTag, FInventoryItemData, ItemData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnItemUnequippedSignature, FGameplayTag, SlotTag, TSoftObjectPtr<UItemData>, Asset);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnEquipmentSlotQuantityUpdatedSignature, FGameplayTag, SlotTag, const FInventoryItemData&, ItemData);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAssignButtonPressed, FGameplayTag, SlotTag, FInventoryItemData, ItemData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEquipButtonPressed, FGameplayTag, InputTag);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAttributePointSpent, FGameplayTag, AttributeTag);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSkillPointSpent, FGameplayTag, AbilityTag);

USTRUCT()
struct FEquippedItem
{
	GENERATED_BODY()

	UPROPERTY()
	FInventoryItemData ItemData = FInventoryItemData();

	FAbilitySet_GrantedHandles OutGrantedHandles = FAbilitySet_GrantedHandles();
	
	UPROPERTY()
	TArray<FGameplayModifierInfo> Modifiers;

	UPROPERTY()
	bool bDirty = false;
};

class UItemAbilityInfo;
struct FGameplayTag;
/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class LEYR_API UInventoryWidgetController : public UWidgetController
{
	GENERATED_BODY()

public:
	void BroadcastEquippedItems();
	virtual void BindCallbacksToDependencies() override;
	void Clear(FGameplayTag InputToClear, FGameplayTag SlotToUnequip);
	void Assign(FGameplayTag InputToAssign, FGameplayTag SlotToEquip, FEquippedItem& ItemToEquip);
	UItemData* HasCompatibleItemCostInAmmunitionSlot(const FGameplayTag CostTag);
	bool HasCompatibleItemCostInInventory(const FGameplayTag CostTag, FInventoryItemData& OutItem) const;
	TMap<FGameplayTag, FEquippedItem> GetEquippedItems() { return EquippedItems; }
	void SetupEquippedItems(const TMap<FGameplayTag, FEquippedItem>& ItemsToEquip);

	void UpdateInventorySlots() const;
	
	UFUNCTION(BlueprintCallable)
	void RequestUpdateInventorySlotsOnce();

	UFUNCTION(BlueprintCallable)
	void AssignButtonPressed(FInventoryItemData ItemData, const FGameplayTag InputTag);
	
	UFUNCTION(BlueprintCallable)
	void EquipButtonPressed(FInventoryItemData ItemData);
	
	UFUNCTION(BlueprintCallable)
	void Equip(const FInventoryItemData& ItemData);
	
	UFUNCTION(BlueprintCallable)
	void LootButtonPressed(int32 SourceSlotIndex) const;
	
	UFUNCTION(BlueprintCallable)
	void LootAllButtonPressed() const;
	
	UFUNCTION(BlueprintCallable)
	void SetContainerOpen(bool bIsOpen) { bContainerIsOpen = bIsOpen; }

	UPROPERTY(BlueprintAssignable)
	FOnInputAssignedSignature OnInputAssigned;
	
	UPROPERTY(BlueprintAssignable)
	FOnInputRemovedSignature OnInputRemoved;

	UPROPERTY(BlueprintAssignable)
	FOnItemEquippedSignature OnItemEquipped;
	
	UPROPERTY(BlueprintAssignable)
	FOnItemUnequippedSignature OnItemUnequipped;
	
	UPROPERTY(BlueprintAssignable)
	FOnEquipmentSlotQuantityUpdatedSignature OnEquipmentSlotQuantityUpdated;

	UPROPERTY(BlueprintAssignable)
	FOnPlayerStatChangedSignature OnPlayerLevelChanged;
	
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnXPPercentChanged;
	
	UPROPERTY(BlueprintAssignable)
	FOnXPValueChanged OnXPValueUpdated;

	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<UInventoryComponent> InventoryComponent;

private:
	void UpdateEquipmentEffect();
	void UpdateMonkAbility();
	void UpdateAmmunitionCounter(FGameplayTag Slot, FEquippedItem EquippedItem) const;
	void UpdateItemAbilities();
	void OnXPChanged(int32 NewXP);
	void AsyncUpdateAbilities(TSoftObjectPtr<UItemData> AssetToLoad, FAbilitySet_GrantedHandles* OutGrantedHandles, FGameplayTag InputTag);
	TArray<FEquippedItem> GetEquippedActionSlots(TMap<FGameplayTag, FEquippedItem> Items);
	
	TMap<FGameplayTag, FEquippedItem> EquippedItems;
	TMap<FGameplayTag, FEquippedItem> PreviouslyEquippedItems;
	FActiveGameplayEffectHandle ActiveEquipmentEffectHandle;
	bool bContainerIsOpen = false;
	bool bRequestUpdateInventorySlots = true;
};
