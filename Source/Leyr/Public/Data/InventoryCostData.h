// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "InventoryCostData.generated.h"

class UItemData;

USTRUCT(BlueprintType)
struct FInventoryCost
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(Categories="Cost"))
	FGameplayTag CostTag = FGameplayTag();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<UItemData*>  CompatibleItems;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bProducesRefund = false;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<UItemData*>  RefundedItems;
};

/**
 * 
 */
UCLASS()
class LEYR_API UInventoryCostData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CostInformation")
	TArray<FInventoryCost> InventoryCostInformation;

	FInventoryCost FindCostInfoForTag(const FGameplayTag& CostTag, bool bLogNotFound = false) const;
};
