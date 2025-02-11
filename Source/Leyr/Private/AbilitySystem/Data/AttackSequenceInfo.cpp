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
	case ESequenceType::TwoHanded: Sequences = TwoHandedSequences;
		break;
	case ESequenceType::Dagger: Sequences = DaggerSequences;
		break;
	case ESequenceType::Piercing: Sequences = PiercingSequences;
		break;
	case ESequenceType::Use: return UseSequence;
	case ESequenceType::Throw: return ThrowSequence;
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
	case ESequenceType::TwoHanded: return TwoHandedSequences;
	case ESequenceType::Dagger: return DaggerSequences;
	case ESequenceType::Piercing: return PiercingSequences;
	case ESequenceType::Use: return TArray{ UseSequence };
	case ESequenceType::Throw: return TArray{ ThrowSequence };
	}
	return DefaultSequences;
}
