// @ Retropsis 2024-2025.

#include "AbilitySystem/Data/AttackSequenceInfo.h"
#include "LeyrLogChannels.h"
#include "Data/CombatData.h"

FTaggedMontage UAttackSequenceInfo::FindSequenceInfoForTag(const FGameplayTag& MontageTag, ESequenceType SequenceType, bool bLogNotFound) const
{
	TArray<FTaggedMontage> Sequences;
	
	switch (SequenceType)
	{
	case ESequenceType::Default: Sequences = DefaultSequences;
		break;
	case ESequenceType::MartialArt: Sequences = MartialArtSequences;
		break;
	case ESequenceType::OneHanded: Sequences = OneHandedSequences;
		break;
	case ESequenceType::TwoHanded: Sequences = MartialArtSequences;
		break;
	}
	
	for (const FTaggedMontage& Info : Sequences)
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

TArray<FTaggedMontage> UAttackSequenceInfo::GetSequencesByType(ESequenceType SequenceType)
{
	switch (SequenceType)
	{
	case ESequenceType::Default: return DefaultSequences;
	case ESequenceType::MartialArt: return MartialArtSequences;
	case ESequenceType::OneHanded: return OneHandedSequences;
	case ESequenceType::TwoHanded: return MartialArtSequences;
	}
	return DefaultSequences;
}
