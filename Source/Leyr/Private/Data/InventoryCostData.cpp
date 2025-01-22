// @ Retropsis 2024-2025.

#include "Data/InventoryCostData.h"
#include "LeyrLogChannels.h"

FInventoryCost UInventoryCostData::FindCostInfoForTag(const FGameplayTag& CostTag, bool bLogNotFound) const
{
	for (const FInventoryCost& Info : InventoryCostInformation)
	{
		if (Info.CostTag == CostTag)
		{
			return Info;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogLeyr, Error, TEXT("Can't find info for CostTag [%s] on InventoryCostInfo [%s]"), *CostTag.ToString(), *GetNameSafe(this));
	}

	return FInventoryCost();
}
