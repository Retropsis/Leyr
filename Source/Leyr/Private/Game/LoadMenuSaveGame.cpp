// @ Retropsis 2024-2025.


#include "Game/LoadMenuSaveGame.h"

FMapSaveData ULoadMenuSaveGame::GetMapSaveDataWithName(const FString& InName)
{
	for (const FMapSaveData& Map : SavedMaps)
	{
		if (Map.AssetName == InName)
		{
			return Map;
		}
	}
	return FMapSaveData();
}

bool ULoadMenuSaveGame::HasMap(const FString& InName)
{
	for (const FMapSaveData& Map : SavedMaps)
	{
		if (Map.AssetName == InName)
		{
			return true;
		}
	}
	return false;
}
