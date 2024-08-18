// @ Retropsis 2024-2025.

#include "Inventory/InventoryComponent.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	Items.SetNum(Columns * Rows);
}

bool UInventoryComponent::TryAddItem(FInventoryItemData ItemToAdd)
{
	if (ItemToAdd.ID == 0) return false;

	for (int i = 0; i < Items.Num(); ++i)
	{
		if (IsRoomAvailable(ItemToAdd, i))
		{
			AddItemAt(ItemToAdd, i);
			return true;
		}
	}
	return false;
}

void UInventoryComponent::AddItemAt(FInventoryItemData ItemToAdd, int32 TopLeftIndex)
{
	const int32 FirstI = IndexToTile(TopLeftIndex).X;
	const int32 LastI = FirstI + ItemToAdd.Dimensions.X - 1;

	const int32 FirstJ = IndexToTile(TopLeftIndex).Y;
	const int32 LastJ = FirstJ + ItemToAdd.Dimensions.Y - 1;
	
	for (int i = FirstI; i < LastI; ++i)
	{
		for (int j = FirstJ; j < LastJ; ++j)
		{
			Items[TileToIndex(FTile(i, j))] = ItemToAdd;
		}
	}
	bIsDirty = true;
}

bool UInventoryComponent::IsRoomAvailable(FInventoryItemData ItemToAdd, int32 TopLeftIndex)
{
	const int32 FirstI = IndexToTile(TopLeftIndex).X;
	const int32 LastI = FirstI + ItemToAdd.Dimensions.X - 1;

	const int32 FirstJ = IndexToTile(TopLeftIndex).Y;
	const int32 LastJ = FirstJ + ItemToAdd.Dimensions.Y - 1;
	
	for (int i = FirstI; i < LastI; ++i)
	{
		for (int j = FirstJ; j < LastJ; ++j)
		{
			if(IsTileValid(FTile(i, j)))
			{
				FInventoryItemData Item = GetItemAtIndex(TileToIndex(FTile(i, j)));
				if(Item.ID != 0) return false;
			}
			else
			{
				return false;
			}
		}
	}
	return true;
}

// FTile UInventoryComponent::ForEachIndex(FInventoryItemData ItemToAdd, int32 TopLeftIndex)
// {
// 	const int32 FirstI = IndexToTile(TopLeftIndex).X;
// 	const int32 LastI = FirstI + ItemToAdd.Dimensions.X - 1;
//
// 	const int32 FirstJ = IndexToTile(TopLeftIndex).Y;
// 	const int32 LastJ = FirstJ + ItemToAdd.Dimensions.Y - 1;
// 	
// 	for (int i = FirstI; i < LastI; ++i)
// 	{
// 		for (int j = FirstJ; j < LastJ; ++j)
// 		{
// 			return FTile(i, j);
// 		}
// 	}
// }

FTile UInventoryComponent::IndexToTile(int32 Index) const { return FTile{ FMath::Modulo(Index, Columns), Index / Columns }; }
int32 UInventoryComponent::TileToIndex(FTile Tile) const { return Tile.X + Tile.Y * Columns; }
bool UInventoryComponent::IsTileValid(const FTile Tile) const { return Tile.X >= 0 && Tile.Y >= 0 && Tile.X < Columns && Tile.Y < Rows; }

FInventoryItemData UInventoryComponent::GetItemAtIndex(int32 Index)
{
	if (Items.IsValidIndex(Index))
	{
		return Items[Index];
	}
	return FInventoryItemData();
}
