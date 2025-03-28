// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LootDataSet.generated.h"

USTRUCT(BlueprintType)
struct FLootData
{
	GENERATED_BODY()
 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Loot")
	TSubclassOf<AActor> ItemClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Loot")
	FDataTableRowHandle ItemRowHandle;
 
	UPROPERTY(EditDefaultsOnly, Category="Loot")
	float SpawningChance = 0.f;
 
	UPROPERTY(EditDefaultsOnly, Category="Loot")
	int32 MaxNumberToSpawn = 0.f;
 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Loot")
	bool bLootLevelOverride = true;
};

/**
 * 
 */
UCLASS()
class LEYR_API ULootDataSet : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	TArray<FLootData> GetLootItems();
 	
	UPROPERTY(EditDefaultsOnly, Category="Loot")
	TArray<FLootData> LootItems;
};
