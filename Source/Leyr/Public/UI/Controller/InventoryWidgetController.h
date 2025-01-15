// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Inventory/InventoryComponent.h"
#include "UI/Controller/WidgetController.h"
#include "InventoryWidgetController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnInputAssignedSignature, int32, ItemID, FGameplayTag, InputTag);

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
	
	UFUNCTION(BlueprintCallable)
	void EquipButtonPressed(FInventoryItemData ItemData, const FGameplayTag& InputTag);
	
	void ClearEquipButtonByItemData(const FInventoryItemData& ItemData);
	void ClearEquipButtonByInputTag(const FGameplayTag InputTag);

	UFUNCTION()
	void HandleItemUpdated(EContainerType ContainerType, int32 SlotIndex, FInventoryItemData Item);

	UPROPERTY(BlueprintAssignable)
	FOnInputAssignedSignature OnInputAssigned;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UInventoryComponent> InventoryComponent;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget Data")
	TObjectPtr<UItemAbilityInfo> ItemAbilityInfo;

private:
	TMap<FGameplayTag, FInventoryItemData> EquippedItemAbilities;
};
