// @ Retropsis 2024-2025.

#include "AbilitySystem/Ability/MasteryAbility.h"
#include "AbilitySystem/Data/AbilityDescriptionData.h"

FString UMasteryAbility::GetDescription(int32 Level)
{
	// const UAbilityInfo* AbilityInfo =  ULeyrAbilitySystemLibrary::GetAbilityInfo(GetWorld());
	// if(AbilityInfo == nullptr) return FString();

	// const FBaseAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(GetCurrentAbilitySpec()->Ability->AbilityTags.First());
	
	FString CurrentLevel = FString::Printf(TEXT("<Title>%s Lv.</><Level>%d</>\n"), *Info.Name, Level);

	if(Info.CustomValue > 0.f)
	{
		CurrentLevel.Appendf(TEXT("<Default>%s</><Damage>%.2f</>\n\n"), *Info.CustomValueText, 1.f + Info.CustomValue * Level);
	}
	
	const int32 Damage = AbilityPower.GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	
	return CurrentLevel;
}

FString UMasteryAbility::GetNextLevelDescription(int32 Level)
{
	// const UAbilityInfo* AbilityInfo =  ULeyrAbilitySystemLibrary::GetAbilityInfo(GetWorld());
	// if(AbilityInfo == nullptr) return FString();
	//
	// const FBaseAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(GetCurrentAbilitySpec()->Ability->AbilityTags.First());
	
	FString CurrentLevel = FString::Printf(TEXT("<Title>%s Lv.</><Level>%d</>\n"), *Info.Name, Level);

	if(Info.CustomValue > 0.f)
	{
		CurrentLevel.Appendf(TEXT("<Default>%s</><Damage>%.2f</>\n\n"), *Info.CustomValueText, 1.f + Info.CustomValue * Level);
	}
	
	const int32 Damage = AbilityPower.GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	
	return CurrentLevel;
}
