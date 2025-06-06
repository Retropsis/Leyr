// @ Retropsis 2024-2025.

#include "AbilitySystem/Data/AttributeInfo.h"

FBaseAttributeInfo UAttributeInfo::FindAttributeInfoForTag(const FGameplayTag& AttributeTag, bool bLogNotFound) const
{
	for (const FBaseAttributeInfo& Info : AttributeInformation)
	{
		if (Info.AttributeTag.MatchesTagExact(AttributeTag))
		{
			return Info;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogTemp, Error, TEXT("Can't find Info for AttributeTag [%s] on AttributeInfo [%s]."), *AttributeTag.ToString(),*GetNameSafe(this));
	}
	return FBaseAttributeInfo();
}

FBaseAttributeInfo UAttributeInfo::FindAttributeInfoForName(const FString& Name, bool bLogNotFound) const
{
	for (const FBaseAttributeInfo& Info : AttributeInformation)
	{
		if (Info.AttributeGetter.AttributeName == Name)
		{
			return Info;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogTemp, Error, TEXT("Can't find Info for AttributeName [%s] on AttributeInfo [%s]."), *Name,*GetNameSafe(this));
	}
	return FBaseAttributeInfo();
}
