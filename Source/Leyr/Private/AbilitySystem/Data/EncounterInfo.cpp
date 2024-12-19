// @ Retropsis 2024-2025.

#include "AbilitySystem/Data/EncounterInfo.h"

FEncounterDefaultInfo UEncounterInfo::GetEncounterDefaultInfo(FName EncounterName)
{
	return EncounterInformation.FindChecked(EncounterName);
}
