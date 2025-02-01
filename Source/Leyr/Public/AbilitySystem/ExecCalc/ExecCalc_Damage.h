// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "Game/BaseGameplayTags.h"
#include "ExecCalc_Damage.generated.h"

/**
 * 
 */
UCLASS()
class LEYR_API UExecCalc_Damage : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
	
public:
	UExecCalc_Damage();

	float ApplyMasteryEffects(const FGameplayEffectCustomExecutionParameters& ExecutionParams) const;
	FActiveGameplayEffectHandle ApplyEquipmentEffects(const FGameplayEffectCustomExecutionParameters& ExecutionParams) const;
	
	void DetermineStatusEffect(const FGameplayEffectCustomExecutionParameters& ExecutionParams, const FGameplayEffectSpec& Spec, FAggregatorEvaluateParameters EvaluationParameters,
					 const TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition>& InTagsToDefs) const;
	
	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
