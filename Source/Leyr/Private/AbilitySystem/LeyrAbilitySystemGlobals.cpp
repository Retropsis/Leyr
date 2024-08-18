// @ Retropsis 2024-2025.

#include "AbilitySystem/LeyrAbilitySystemGlobals.h"
#include "AbilityTypes.h"

FGameplayEffectContext* ULeyrAbilitySystemGlobals::AllocGameplayEffectContext() const
{
	return new FBaseGameplayEffectContext();
}
