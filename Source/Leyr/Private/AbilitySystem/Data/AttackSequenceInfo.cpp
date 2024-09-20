// @ Retropsis 2024-2025.

#include "AbilitySystem/Data/AttackSequenceInfo.h"
#include "LeyrLogChannels.h"
#include "Data/CombatData.h"

FTaggedMontage UAttackSequenceInfo::FindSequenceInfoForTag(const FGameplayTag& MontageTag, bool bLogNotFound) const
{
	for (const FTaggedMontage& Info : OneHandedSequences)
	{
		if (Info.MontageTag == MontageTag)
		{
			return Info;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogLeyr, Error, TEXT("Can't find info for MontageTag [%s] on AttackSequenceInfo [%s]"), *MontageTag.ToString(), *GetNameSafe(this));
	}

	return FTaggedMontage();
}
