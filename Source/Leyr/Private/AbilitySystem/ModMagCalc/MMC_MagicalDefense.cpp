// @ Retropsis 2024-2025.

#include "AbilitySystem/ModMagCalc/MMC_MagicalDefense.h"
#include "AbilitySystem/BaseAttributeSet.h"

UMMC_MagicalDefense::UMMC_MagicalDefense()
{
	SpiritDef.AttributeToCapture = UBaseAttributeSet::GetSpiritAttribute();
	SpiritDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	SpiritDef.bSnapshot = false;
	
	MagicalDefenseBaseDef.AttributeToCapture = UBaseAttributeSet::GetMagicalDefenseBaseAttribute();
	MagicalDefenseBaseDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	MagicalDefenseBaseDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(MagicalDefenseBaseDef);
	RelevantAttributesToCapture.Add(SpiritDef);
}

float UMMC_MagicalDefense::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	// Gather tags from source and target
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float Spirit = 0.f;
	GetCapturedAttributeMagnitude(SpiritDef, Spec, EvaluationParameters, Spirit);
	Spirit = FMath::Max<float>(Spirit, 0.f);

	float MagicalDefenseBase = 0.f;
	GetCapturedAttributeMagnitude(MagicalDefenseBaseDef, Spec, EvaluationParameters, MagicalDefenseBase);
	MagicalDefenseBase = FMath::Max<float>(MagicalDefenseBase, 0.f);
		
	return (2.f + Spirit) * 0.25f + 6.f + MagicalDefenseBase;
}
