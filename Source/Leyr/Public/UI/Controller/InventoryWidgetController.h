// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "ActiveGameplayEffectHandle.h"
#include "GameplayTagContainer.h"
#include "Inventory/InventoryComponent.h"
#include "UI/Controller/WidgetController.h"
#include "InventoryWidgetController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnInputAssignedSignature, FInventoryItemData, ItemData, FGameplayTag, InputTag);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInputRemovedSignature, FGameplayTag, InputTag);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnItemEquippedSignature, FGameplayTag, SlotTag, FInventoryItemData, ItemData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnItemUnequippedSignature, FGameplayTag, SlotTag, TSoftObjectPtr<UItemData>, Asset);

USTRUCT()
struct FEquippedItem
{
	GENERATED_BODY()

	FInventoryItemData ItemData = FInventoryItemData();
	TArray<FGameplayTag> Abilities;
	TArray<FGameplayModifierInfo> Modifiers;
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
	virtual void BindCallbacksToDependencies() override;
	void Clear(FGameplayTag InputToClear, FGameplayTag SlotToUnequip);
	void Assign(FGameplayTag InputToAssign, FGameplayTag SlotToEquip, FEquippedItem& ItemToEquip);
	UItemData* HasCompatibleItemCostInAmmunitionSlot(const FGameplayTag CostTag);

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

	UFUNCTION()
	void HandleItemUpdated(EContainerType ContainerType, int32 SlotIndex, FInventoryItemData Item);

	UPROPERTY(BlueprintAssignable)
	FOnInputAssignedSignature OnInputAssigned;
	
	UPROPERTY(BlueprintAssignable)
	FOnInputRemovedSignature OnInputRemoved;

	UPROPERTY(BlueprintAssignable)
	FOnItemEquippedSignature OnItemEquipped;
	
	UPROPERTY(BlueprintAssignable)
	FOnItemUnequippedSignature OnItemUnequipped;

	UPROPERTY(BlueprintAssignable)
	FOnPlayerStatChangedSignature OnPlayerLevelChanged;
	
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnXPPercentChanged;
	
	UPROPERTY(BlueprintAssignable)
	FOnXPValueChanged OnXPValueUpdated;

	UPROPERTY(BlueprintAssignable)
	FOnItemUpdated HandleOnItemUpdated;

	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<UInventoryComponent> InventoryComponent;

private:
	void UpdateEquipmentEffect();
	void UpdateMonkAbility();
	void UpdateItemAbilities();
	void OnXPChanged(int32 NewXP);
	
	TMap<FGameplayTag, FEquippedItem> EquippedItems;
	TMap<FGameplayTag, FEquippedItem> PreviouslyEquippedItems;
	FActiveGameplayEffectHandle ActiveEquipmentEffectHandle;
	bool bContainerIsOpen = false;
};
