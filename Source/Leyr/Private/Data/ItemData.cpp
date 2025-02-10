// @ Retropsis 2024-2025.

#include "Data/ItemData.h"
#include "LeyrLogChannels.h"

FTaggedMontage UItemData::FindSequenceInfoForTag(const FGameplayTag& MontageTag, bool bLogNotFound) const
{	
	for (const FTaggedMontage& Info : DefaultSequences)
	{
		if (Info.MontageTag == MontageTag)
		{
			return Info;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogLeyr, Error, TEXT("Can't find info for MontageTag [%s] on ItemData [%s]"), *MontageTag.ToString(), *GetNameSafe(this));
	}

	return FTaggedMontage();
}

float UItemData::GetPrimaryAttributeMagnitude()
{
	for (FGameplayModifierInfo& Info : Modifiers)
	{
		float Magnitude;
		Info.ModifierMagnitude.GetStaticMagnitudeIfPossible(1.f, Magnitude);
		return Magnitude;
	}
	return 0.f;
}
