// @ Retropsis 2024-2025.

#include "AbilitySystem/Ability/Electrocute.h"

FString UElectrocute::GetDescription(int32 Level)
{
	const int32 ScaledDamage = AbilityPower.GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	if (Level == 1)
	{
		return FString::Printf(TEXT(
			"<Title>ELECTROCUTE</>\n\n"
			"<Small>Level: </><Level>%d</>\n"
			"<Small>ManaCost: </><ManaCost>%.1f</>\n"
			"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"
 			
			"<Default>Emits a beam of lightning, "
			"connecting with the target, repeatedly causing </>"
			"<Damage>%d</><Default> lightning damage with"
			" a chance to stun</>"),
			Level, ManaCost, Cooldown, ScaledDamage);
	}
	return FString::Printf(TEXT(
		"<Title>ELECTROCUTE</>\n\n"
		"<Small>Level: </><Level>%d</>\n"
		"<Small>ManaCost: </><ManaCost>%.1f</>\n"
		"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"
		"<Default>Emits a beam of lightning, "
		"propagating to %d additional targets nearby, causing </>"
		"<Damage>%d</><Default> lightning damage with"
		" a chance to stun</>"),
		Level, ManaCost, Cooldown, FMath::Min(Level, MaxNumShockTargets - 1), ScaledDamage);	
}

FString UElectrocute::GetNextLevelDescription(int32 Level)
{
	const int32 ScaledDamage = AbilityPower.GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	return FString::Printf(TEXT(
			"<Title>NEXT LEVEL:</>\n\n"
			"<Small>Level: </><Level>%d</>\n"
			"<Small>ManaCost: </><ManaCost>%.1f</>\n"
			"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"
			"<Default>Emits a beam of lightning, "
			"propagating to %d additional targets nearby, causing </>"
			"<Damage>%d</><Default> lightning damage with"
			" a chance to stun</>"),
			Level, ManaCost, Cooldown, FMath::Min(Level, MaxNumShockTargets - 1), ScaledDamage);	
}
