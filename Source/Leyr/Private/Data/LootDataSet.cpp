// @ Retropsis 2024-2025.

#include "Data/LootDataSet.h"

TArray<FLootData> ULootDataSet::GetLootItems()
{
	TArray<FLootData> ItemsToDrop;

	for (FLootData& Item : LootItems)
	{
		for (int i = 0; i < Item.MaxNumberToSpawn; ++i)
		{
			if (FMath::FRandRange(1.f, 100.f) <= Item.SpawningChance)
			{
				FLootData LootItem;				
				LootItem.ItemClass = Item.ItemClass;
				LootItem.ItemRowHandle = Item.ItemRowHandle;
				LootItem.bLootLevelOverride = Item.bLootLevelOverride;
				LootItem.bShouldDespawn = Item.bShouldDespawn;
				ItemsToDrop.Add(LootItem);
			}
		}
	}
	return ItemsToDrop;;
}
