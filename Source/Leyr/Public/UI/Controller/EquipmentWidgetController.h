// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "Inventory/InventoryComponent.h"
#include "UI/Controller/WidgetController.h"
#include "EquipmentWidgetController.generated.h"

struct FGameplayTag;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnItemEquipped, FGameplayTag, SlotTag, FInventoryItemData, ItemData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemUnequipped, FGameplayTag, SlotTag);

/**
 * 
 */
UCLASS(Blueprintable)
class LEYR_API UEquipmentWidgetController : public UWidgetController
{
	GENERATED_BODY()

public:
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnItemEquipped OnItemEquipped;
	
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnItemUnequipped OnItemUnequipped;

	UFUNCTION(BlueprintCallable)
	void Equip(const FInventoryItemData& ItemData, FGameplayTag InputTag);
	
	UFUNCTION(BlueprintCallable)
	void Unequip(FGameplayTag InputTag);

	TMap<FGameplayTag, FInventoryItemData> EquippedItems;
};
