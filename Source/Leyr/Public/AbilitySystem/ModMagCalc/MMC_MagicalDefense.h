// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "MMC_MagicalDefense.generated.h"

/**
 * 
 */
UCLASS()
class LEYR_API UMMC_MagicalDefense : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()
	
public:
	UMMC_MagicalDefense();
	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

private:
	FGameplayEffectAttributeCaptureDefinition MagicalDefenseBaseDef;
	FGameplayEffectAttributeCaptureDefinition SpiritDef;	
};
