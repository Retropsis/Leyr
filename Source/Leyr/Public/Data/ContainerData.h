// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "ItemDataRow.h"
#include "Engine/DataAsset.h"
#include "ContainerData.generated.h"

class UPaperFlipbook;
/**
 * 
 */
USTRUCT(Blueprintable, BlueprintType)
struct FContainerItem
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FDataTableRowHandle Item = FDataTableRowHandle();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 Quantity = 1;
	
	FInventoryItemData ItemData = FInventoryItemData();
};

UCLASS(Blueprintable, BlueprintType)
class LEYR_API UContainerData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<FContainerItem> Items;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<UPaperFlipbook> ContainerFlipbook;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<USoundBase> OpenLidSound = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<USoundBase> CloseLidSound = nullptr;
};
