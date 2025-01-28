// @ Retropsis 2024-2025.

#include "AbilitySystem/Data/PassiveInfo.h"
#include "LeyrLogChannels.h"

FBasePassiveInfo UPassiveInfo::FindPassiveInfoForTag(const FGameplayTag& AbilityTag, bool bLogNotFound) const
{
	for (const FBasePassiveInfo& Info : PassiveInformation)
	{
		if (Info.AbilityTag == AbilityTag)
		{
			return Info;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogLeyr, Error, TEXT("Can't find info for AbilityTag [%s] on PassiveInfo [%s]"), *AbilityTag.ToString(), *GetNameSafe(this));
	}

	return FBasePassiveInfo();
}
