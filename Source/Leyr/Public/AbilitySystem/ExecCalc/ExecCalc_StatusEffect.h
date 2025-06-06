// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "ExecCalc_StatusEffect.generated.h"

/**
 * 
 */
UCLASS()
class LEYR_API UExecCalc_StatusEffect : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
	
public:
	UExecCalc_StatusEffect();

	// float ApplyMasteryEffects(const FGameplayEffectCustomExecutionParameters& ExecutionParams) const;
	// FActiveGameplayEffectHandle ApplyEquipmentEffects(const FGameplayEffectCustomExecutionParameters& ExecutionParams) const;
	
	void DetermineStatusEffect(const FGameplayEffectCustomExecutionParameters& ExecutionParams, const FGameplayEffectSpec& Spec, const FAggregatorEvaluateParameters& EvaluationParameters,
					 const TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition>& InTagsToDefs) const;
	
	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
