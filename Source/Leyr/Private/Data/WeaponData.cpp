// @ Retropsis 2024-2025.

#include "Data/WeaponData.h"
#include "AbilitySystem/Data/AttributeInfo.h"

UWeaponData::UWeaponData()
{
	bIsStackable = false;
	ItemType = EItemType::Equipment;
}

FString UWeaponData::GetDescription(UAttributeInfo* AttributeInfo, int32 Level)
{
	FString Rows;
	for (FGameplayModifierInfo ModifierInfo : Modifiers)
	{
		FString AttributeName = ModifierInfo.Attribute.AttributeName;
		if (AttributeInfo)
		{
			FBaseAttributeInfo Info = AttributeInfo->FindAttributeInfoForName(ModifierInfo.Attribute.AttributeName);
			AttributeName = Info.AttributeName.ToString();
		}
		float Magnitude = 0.f;
		ModifierInfo.ModifierMagnitude.GetStaticMagnitudeIfPossible(1.f, Magnitude);
		FString Row = FString::Printf(TEXT("<Default>%s: </><Level>%.0f</>\n"), *AttributeName, Magnitude);
		Rows.Append(Row);
	}
	return FString::Printf(TEXT(
		"<Title>%s</>\n"
		"%s\n"
		"%s\n"
		// "<Small>ManaCost: </><ManaCost>%.1f</>\n"
		// "<Small>Cooldown: </><Cooldown>%.1f</>\n\n"
		// "<Default>Emits a beam of lightning, "
		// "propagating to %d additional targets nearby, causing </>"
		""),
		*Name.ToString(), *Description.ToString(), *Rows);	
}
