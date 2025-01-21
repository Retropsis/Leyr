// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "MMC_PhysicalDefense.generated.h"

/**
 * 
 */
UCLASS()
class LEYR_API UMMC_PhysicalDefense : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()
	
public:
	UMMC_PhysicalDefense();
	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

private:
	FGameplayEffectAttributeCaptureDefinition PhysicalDefenseBaseDef;
	FGameplayEffectAttributeCaptureDefinition VitalityDef;	
};
