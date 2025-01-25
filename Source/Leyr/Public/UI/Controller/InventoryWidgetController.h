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
	
	FActiveGameplayEffectHandle ApplyExecuteEffectToSelf(UGameplayEffect* EffectToApply, const UObject* SourceObject, const FGameplayTag EquipmentSlot, int32 Level = 1);
	FActiveGameplayEffectHandle MakeAndApplyEffectToSelf(const UObject* SourceObject, const FGameplayTag& EquipmentSlot, TArray<FGameplayModifierInfo> Modifiers, int32 Level = 1);
	void RemoveActiveGameplayEffect(FGameplayTag EquipmentSlot);

	void UpdateEquipmentEffect();

	UFUNCTION(BlueprintCallable)
	void AssignButtonPressed(FInventoryItemData ItemData, const FGameplayTag InputTag);
	
	UFUNCTION(BlueprintCallable)
	void EquipButtonPressed(FInventoryItemData ItemData);
	
	UFUNCTION(BlueprintCallable)
	void Equip(const FInventoryItemData& ItemData);
	
	UFUNCTION(BlueprintCallable)
	void LootButtonPressed(int32 SourceSlotIndex);
	
	UFUNCTION(BlueprintCallable)
	void LootAllButtonPressed();
	
	UFUNCTION(BlueprintCallable)
	void SetContainerOpen(bool bIsOpen) { bContainerIsOpen = bIsOpen; }

	UFUNCTION()
	void HandleItemUpdated(EContainerType ContainerType, int32 SlotIndex, FInventoryItemData Item);
	
	UPROPERTY(EditDefaultsOnly, Category="WidgetController|Equipment")
	TSubclassOf<UGameplayEffect> EquipmentEffectClass;

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
	FActiveGameplayEffectHandle ActiveEquipmentEffectHandle;
	bool bContainerIsOpen = false;
};
