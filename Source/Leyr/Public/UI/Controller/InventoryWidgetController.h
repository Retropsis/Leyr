// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Inventory/InventoryComponent.h"
#include "UI/Controller/WidgetController.h"
#include "InventoryWidgetController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnInputAssignedSignature, FInventoryItemData, ItemData, FGameplayTag, InputTag);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInputRemovedSignature, FGameplayTag, InputTag);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEquipmentAssignedSignature, FInventoryItemData, ItemData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEquipmentRemovedSignature, FInventoryItemData, ItemData);

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
	void ClearInputTag(FInventoryItemData ItemData, const FGameplayTag& InputTag);
	bool ReplaceInputTag(FInventoryItemData ItemData, const FGameplayTag& InputTag);
	void AssignInputTag(FInventoryItemData ItemData, const FGameplayTag& InputTag);

	UFUNCTION(BlueprintCallable)
	void AssignButtonPressed(FInventoryItemData ItemData, const FGameplayTag& InputTag);
	
	UFUNCTION(BlueprintCallable)
	void EquipButtonPressed(FInventoryItemData ItemData);
	
	UFUNCTION(BlueprintCallable)
	void LootButtonPressed(int32 SourceSlotIndex);
	
	UFUNCTION(BlueprintCallable)
	void LootAllButtonPressed();
	
	UFUNCTION(BlueprintCallable)
	void SetContainerOpen(bool bIsOpen) { bContainerIsOpen = bIsOpen; }
	
	void ClearEquipButtonByItemData(const FInventoryItemData& ItemData);
	bool ClearEquipButtonByInputTag(const FGameplayTag InputTag, const int32 ItemID);

	UFUNCTION()
	void HandleItemUpdated(EContainerType ContainerType, int32 SlotIndex, FInventoryItemData Item);

	UPROPERTY(BlueprintAssignable)
	FOnInputAssignedSignature OnInputAssigned;
	
	UPROPERTY(BlueprintAssignable)
	FOnInputRemovedSignature OnInputRemoved;

	UPROPERTY(BlueprintAssignable)
	FOnEquipmentAssignedSignature OnEquipmentAssigned;
	
	UPROPERTY(BlueprintAssignable)
	FOnEquipmentRemovedSignature OnEquipmentRemoved;

	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<UInventoryComponent> InventoryComponent;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget Data")
	TObjectPtr<UItemAbilityInfo> ItemAbilityInfo;

private:
	TMap<FGameplayTag, FInventoryItemData> EquippedItemAbilities;
	bool bContainerIsOpen = false;
};
