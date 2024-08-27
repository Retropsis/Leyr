// @ Retropsis 2024-2025.

#include "AbilitySystem/Ability/BowAbility.h"

FString UBowAbility::GetDescription(int32 Level)
{
	const int32 MinDamage = AbilityPower.ValueMin.GetValueAtLevel(Level);
	const int32 MaxDamage = AbilityPower.ValueMax.GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	if(Level == 1)
	{
		return FString::Printf(TEXT(
			"<Title>BOW SHOT: </>"
			"<Small>Level: </><Level>%d</><Default> - </>"
			"<Small>MP: </><ManaCost>%.1f</><Default> - </>"
			"<Small>CD: </><Cooldown>%.1f</>\n"
			
			"<Default>Draws and shoot an arrow, exploding on impact and dealing: </>"
			"<Damage>%d-%d</>"
			"<Default> piercing damage with a chance to burn.</>\n\n"
			), Level, ManaCost, Cooldown, MinDamage, MaxDamage);
	}
	return FString::Printf(TEXT(
		"<Title>FIRE BOLT: </>"
		"<Small>Level: </><Level>%d</><Default> - </>"
		"<Small>MP: </><ManaCost>%.1f</><Default> - </>"
		"<Small>CD: </><Cooldown>%.1f</>\n"
		
		"<Default>Shoots %d arrows, exploding on impact and dealing: </>"
		"<Damage>%d-%d</>"
		"<Default> piercing damage with a chance to burn.</>\n\n"
		), Level, ManaCost, Cooldown, FMath::Min(Level, NumProjectiles), MinDamage, MaxDamage);
}

FString UBowAbility::GetNextLevelDescription(int32 Level)
{
	const int32 MinDamage = AbilityPower.ValueMin.GetValueAtLevel(Level);
	const int32 MaxDamage = AbilityPower.ValueMax.GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	
	return FString::Printf(TEXT(
		"<Title>NEXT LEVEL: </>"
		"<Small>Level: </><Level>%d</><Default> - </>"
		"<Small>MP: </><ManaCost>%.1f</><Default> - </>"
		"<Small>CD: </><Cooldown>%.1f</>\n"
			
		"<Default>Shoots %d arrows, exploding on impact and dealing: </>"
		"<Damage>%d-%d</>"
		"<Default> piercing damage with a chance to burn.</>\n\n"
		), Level, ManaCost, Cooldown, FMath::Min(Level, NumProjectiles), MinDamage, MaxDamage);
}