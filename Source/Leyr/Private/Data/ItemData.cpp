// @ Retropsis 2024-2025.

#include "Data/ItemData.h"

float UItemData::GetPrimaryAttributeMagnitude()
{
	for (FGameplayModifierInfo& Info : Modifiers)
	{
		float Magnitude;
		Info.ModifierMagnitude.GetStaticMagnitudeIfPossible(1.f, Magnitude);
		return Magnitude;
	}
	return 0.f;
}
