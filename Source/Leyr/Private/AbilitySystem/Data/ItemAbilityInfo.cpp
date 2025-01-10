// @ Retropsis 2024-2025.

#include "AbilitySystem/Data/ItemAbilityInfo.h"
#include "LeyrLogChannels.h"

FBaseItemAbilityInfo UItemAbilityInfo::FindItemAbilityInfoForTag(const FGameplayTag& AbilityTag, bool bLogNotFound) const
{
	for (const FBaseItemAbilityInfo& Info : AbilityInformation)
	{
		if (Info.AbilityTag == AbilityTag)
		{
			return Info;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogLeyr, Error, TEXT("Can't find info for AbilityTag [%s] on AbilityInfo [%s]"), *AbilityTag.ToString(), *GetNameSafe(this));
	}

	return FBaseItemAbilityInfo();
}
