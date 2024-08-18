// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemGlobals.h"
#include "LeyrAbilitySystemGlobals.generated.h"

/**
 * 
 */
UCLASS()
class LEYR_API ULeyrAbilitySystemGlobals : public UAbilitySystemGlobals
{
	GENERATED_BODY()
	
	virtual FGameplayEffectContext* AllocGameplayEffectContext() const override;
};
