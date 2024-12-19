// @ Retropsis 2024-2025.

#include "World/Data/MapInfo.h"

FMapDefaultInfo UMapInfo::GetMapDefaultInfo(FName MapName)
{
	return MapInformation.FindChecked(MapName);
}
