// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "ActiveGameplayEffectHandle.h"
#include "GameplayTagContainer.h"
#include "Data/AbilitySet.h"
#include "Inventory/InventoryComponent.h"
#include "UI/Controller/WidgetController.h"
#include "InventoryWidgetController.generated.h"

class UItemAbilityInfo;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnInputAssignedSignature, FInventoryItemData, ItemData, FGameplayTag, InputTag, bool, bPlaySound);
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
};

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
	UItemData* HasCompatibleItemCostInAmmunitionSlot(const FGameplayTag CostTag);
	bool HasCompatibleItemCostInInventory(const FGameplayTag CostTag, FInventoryItemData& OutItem) const;
	TMap<FGameplayTag, FEquippedItem> GetEquippedItems() { return EquippedItems; }

	void UpdateInventorySlots() const;
	
	UFUNCTION(BlueprintCallable)
	void RequestUpdateInventorySlotsOnce();

	UFUNCTION(BlueprintCallable)
	void AssignButtonPressedLocked(FInventoryItemData ItemData);
	
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

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAbilitySet> MonkSet;

private:
	void UpdateEquipmentEffect();
	void UpdateAmmunitionCounter(FGameplayTag Slot, FEquippedItem EquippedItem) const;
	void OnXPChanged(int32 NewXP);
	void AsyncUpdateAbilities(FEquippedItem& ItemToEquip, FGameplayTag InputTag);
	TMap<FGameplayTag, FEquippedItem> GetActionSlots();
	TMap<FGameplayTag, FEquippedItem> GetEquipmentSlots();
	
	TMap<FGameplayTag, FEquippedItem> EquippedItems;
	FActiveGameplayEffectHandle ActiveEquipmentEffectHandle;
	bool bContainerIsOpen = false;
	bool bRequestUpdateInventorySlots = true;
	bool bIsLoading = true;
};
