#pragma once

#include "CoreMinimal.h"
#include "Inventory/InventoryComponent.h"
#include "ItemDataRow.generated.h"

class UPaperFlipbook;

USTRUCT(Blueprintable, BlueprintType)
struct FItemDataRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FInventoryItemData ItemData = FInventoryItemData();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UPaperFlipbook> PickupFlipbook = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<USoundBase> PickupSound = nullptr;
};