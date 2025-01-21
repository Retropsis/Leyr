// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "MMC_PhysicalAttack.generated.h"

/**
 * 
 */
UCLASS()
class LEYR_API UMMC_PhysicalAttack : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()
	
public:
	UMMC_PhysicalAttack();
	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

private:
	FGameplayEffectAttributeCaptureDefinition PhysicalAttackBaseDef;
	FGameplayEffectAttributeCaptureDefinition StrengthDef;	
	
};
