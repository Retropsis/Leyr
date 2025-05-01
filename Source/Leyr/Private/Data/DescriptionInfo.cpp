// @ Retropsis 2024-2025.

#include "Data/DescriptionInfo.h"
#include "LeyrLogChannels.h"

FBaseDescriptionInfo UDescriptionInfo::FindDescriptionInfoForTag(const FGameplayTag& DescriptionTag, bool bLogNotFound) const
{
	for (const FBaseDescriptionInfo& Info : DescriptionInformation)
	{
		if (Info.DescriptionTag == DescriptionTag)
		{
			return Info;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogLeyr, Error, TEXT("Can't find info for DescriptionTag [%s] on DescriptionInfo [%s]"), *DescriptionTag.ToString(), *GetNameSafe(this));
	}
	return FBaseDescriptionInfo();
}
