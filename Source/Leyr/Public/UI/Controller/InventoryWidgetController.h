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
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemUnequippedSignature, FGameplayTag, SlotTag);

USTRUCT()
struct FEquippedItem
{
	GENERATED_BODY()

	FInventoryItemData ItemData = FInventoryItemData();
	FActiveGameplayEffectHandle ActiveEffect = FActiveGameplayEffectHandle();	
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

	bool ReplaceInputTag(FInventoryItemData ItemData, const FGameplayTag InputTag);
	void AssignInputTag(const FInventoryItemData& ItemData, const FGameplayTag InputTag);
	void ClearEquipButtonByItemData(const FInventoryItemData& ItemData);
	bool ClearEquipButtonByInputTag(const FGameplayTag InputTag, const UItemData* Asset);
	void ClearInputTag(const FEquippedItem* EquippedItem, const FGameplayTag InputTag);
	UItemData* HasCompatibleItemCostInAmmunitionSlot(const FGameplayTag CostTag);
	
	FActiveGameplayEffectHandle ApplyExecuteEffectToSelf(UGameplayEffect* EffectToApply, const UObject* SourceObject, const FGameplayTag EquipmentSlot, int32 Level = 1);
	FActiveGameplayEffectHandle MakeAndApplyEffectToSelf(const UObject* SourceObject, const FGameplayTag& EquipmentSlot, TArray<FGameplayModifierInfo> Modifiers, int32 Level = 1);
	void RemoveActiveGameplayEffect(FGameplayTag EquipmentSlot);

	UFUNCTION(BlueprintCallable)
	void AssignButtonPressed(FInventoryItemData ItemData, const FGameplayTag InputTag);
	
	UFUNCTION(BlueprintCallable)
	void EquipButtonPressed(FInventoryItemData ItemData);
	
	UFUNCTION(BlueprintCallable)
	void Assign(const FInventoryItemData& ItemData, FGameplayTag InputTag);
	
	UFUNCTION(BlueprintCallable)
	void Equip(const FInventoryItemData& ItemData);
	
	UFUNCTION(BlueprintCallable)
	void Unequip(FGameplayTag InputTag);
	
	UFUNCTION(BlueprintCallable)
	void LootButtonPressed(int32 SourceSlotIndex);
	
	UFUNCTION(BlueprintCallable)
	void LootAllButtonPressed();
	
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
	FOnItemUpdated HandleOnItemUpdated;

	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<UInventoryComponent> InventoryComponent;

private:
	TMap<FGameplayTag, FEquippedItem> EquippedItems;
	bool bContainerIsOpen = false;
};
