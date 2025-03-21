// @ Retropsis 2024-2025.

#include "AbilitySystem/Data/CharacterInfo.h"

FCharacterDefaultInfo UCharacterInfo::GetCharacterDefaultInfo(FGameplayTag CharacterTag)
{
	return CharacterInformation.FindChecked(CharacterTag);
}
