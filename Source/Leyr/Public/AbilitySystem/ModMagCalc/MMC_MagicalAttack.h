// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "MMC_MagicalAttack.generated.h"

/**
 * 
 */
UCLASS()
class LEYR_API UMMC_MagicalAttack : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()
	
public:
	UMMC_MagicalAttack();
	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

private:
	FGameplayEffectAttributeCaptureDefinition MagicalAttackBaseDef;
	FGameplayEffectAttributeCaptureDefinition IntelligenceDef;	
	
};
