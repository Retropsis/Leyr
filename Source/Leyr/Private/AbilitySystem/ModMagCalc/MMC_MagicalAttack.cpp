// @ Retropsis 2024-2025.

#include "AbilitySystem/ModMagCalc/MMC_MagicalAttack.h"
#include "AbilitySystem/BaseAttributeSet.h"

UMMC_MagicalAttack::UMMC_MagicalAttack()
{
	IntelligenceDef.AttributeToCapture = UBaseAttributeSet::GetVitalityAttribute();
	IntelligenceDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	IntelligenceDef.bSnapshot = false;
	
	MagicalAttackBaseDef.AttributeToCapture = UBaseAttributeSet::GetMagicalAttackBaseAttribute();
	MagicalAttackBaseDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	MagicalAttackBaseDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(MagicalAttackBaseDef);
	RelevantAttributesToCapture.Add(IntelligenceDef);
}

float UMMC_MagicalAttack::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	// Gather tags from source and target
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float Intelligence = 0.f;
	GetCapturedAttributeMagnitude(IntelligenceDef, Spec, EvaluationParameters, Intelligence);
	Intelligence = FMath::Max<float>(Intelligence, 0.f);

	float MagicalAttackBase = 0.f;
	GetCapturedAttributeMagnitude(MagicalAttackBaseDef, Spec, EvaluationParameters, MagicalAttackBase);
	MagicalAttackBase = FMath::Max<float>(MagicalAttackBase, 0.f);
		
	return (2.f + Intelligence) * 0.25f + 6.f + MagicalAttackBase;
}
