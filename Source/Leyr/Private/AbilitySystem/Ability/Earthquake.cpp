// @ Retropsis 2024-2025.

#include "AbilitySystem/Ability/Earthquake.h"

FString UEarthquake::GetDescription(int32 Level)
{
	const int32 ScaledDamage = AbilityPower.GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	if (Level == 1)
	{
		return FString::Printf(TEXT(
			"<Title>Earthquake</>\n\n"
			"<Small>Level: </><Level>%d</>\n"
			"<Small>ManaCost: </><ManaCost>%.1f</>\n"
			"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"
			"<Default>Summon a shard of arcane energy, "
			"causing radial arcane damage of  </>"
			"<Damage>%d</><Default> at the shard origin.</>"),
			Level, ManaCost, Cooldown, ScaledDamage);
	}
	return FString::Printf(TEXT(
			"<Title>ARCANE SHARDS</>\n\n"
			"<Small>Level: </><Level>%d</>\n"
			"<Small>ManaCost: </><ManaCost>%.1f</>\n"
			"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"
			"<Default>Summon %d shards of arcane energy, causing radial arcane damage of </>"
			"<Damage>%d</><Default> at the shard origins.</>"),
			Level, ManaCost, Cooldown, FMath::Min(Level, MaxNumShards), ScaledDamage);	
}

FString UEarthquake::GetNextLevelDescription(int32 Level)
{
	const int32 ScaledDamage = AbilityPower.GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
 	
	return FString::Printf(TEXT(
			"<Title>NEXT LEVEL: </>\n\n"
			"<Small>Level: </><Level>%d</>\n"
			"<Small>ManaCost: </><ManaCost>%.1f</>\n"
			"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"
			"<Default>Summon %d shards of arcane energy, causing radial arcane damage of </>"
			"<Damage>%d</><Default> at the shard origins.</>"),
			Level, ManaCost, Cooldown, FMath::Min(Level, MaxNumShards), ScaledDamage);	
}
