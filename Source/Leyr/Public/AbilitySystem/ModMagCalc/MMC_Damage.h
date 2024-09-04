// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "MMC_Damage.generated.h"

/**
 * 
 */
UCLASS()
class LEYR_API UMMC_Damage : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()
	
public:
	UMMC_Damage();
	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

private:
	FGameplayEffectAttributeCaptureDefinition HealthDef;
	void ShowFloatingText(ACharacter* CharacterInstigator, float Damage, bool bBlockedHit, bool bCriticalHit) const;
};
