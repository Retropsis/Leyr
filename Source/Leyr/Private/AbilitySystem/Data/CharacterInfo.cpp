// @ Retropsis 2024-2025.

#include "AbilitySystem/Data/CharacterInfo.h"

FCharacterDefaultInfo UCharacterInfo::GetCharacterDefaultInfo(ECharacterName CharacterName)
{
	return CharacterInformation.FindChecked(CharacterName);
}
