// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "Grid.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

class AItem;

USTRUCT(BlueprintType)
struct FInventoryItemData
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	int32 ID = 0;

	UPROPERTY(BlueprintReadWrite)
	FIntPoint Dimensions = FIntPoint(2, 3);
	
	UPROPERTY(BlueprintReadWrite)
	int32 Quantity = 0;
	
	UPROPERTY(BlueprintReadWrite)
	int32 StackSize = 0;

	// UPROPERTY(BlueprintReadWrite)
	// TSoftObjectPtr<UItemInfo> Asset = nullptr;

	UPROPERTY(BlueprintReadWrite)
	float Health = 0.f;
	
	UPROPERTY(BlueprintReadWrite)
	float MaxHealth = 0.f;

	UPROPERTY(BlueprintReadWrite)
	TSubclassOf<AItem> ItemClass;
	
	UPROPERTY(BlueprintReadWrite)
	bool bRotated = false;

	UPROPERTY(BlueprintReadWrite)
	UMaterialInterface* Icon;
	
	UPROPERTY(BlueprintReadWrite)
	UMaterialInterface* IconRotated;
};

UENUM(BlueprintType)
enum class EContainerType : uint8
{
	Inventory UMETA(DisplayName="Inventory"),
	Hotbar UMETA(DisplayName="Hotbar"),
	Storage UMETA(DisplayName="Storage"),
	Equipment UMETA(DisplayName="Equipment")
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LEYR_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInventoryComponent();

	bool TryAddItem(FInventoryItemData ItemToAdd);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(ExposeOnSpawn))
	int32 Columns = 5;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(ExposeOnSpawn))
	int32 Rows = 5;

protected:
	virtual void BeginPlay() override;

private:
	void AddItemAt(FInventoryItemData ItemToAdd, int32 TopLeftIndex);
	bool IsRoomAvailable(FInventoryItemData ItemToAdd, int32 TopLeftIndex);
	// FTile ForEachIndex(FInventoryItemData ItemToAdd, int32 TopLeftIndex);
	FTile IndexToTile(int32 Index) const;
	int32 TileToIndex(FTile Tile) const;
	bool IsTileValid(FTile Tile) const;
	FInventoryItemData GetItemAtIndex(int32 Index);
	
	TArray<FInventoryItemData> Items;

	bool bIsDirty = false;
};
