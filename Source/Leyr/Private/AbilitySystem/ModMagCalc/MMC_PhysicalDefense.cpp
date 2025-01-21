// @ Retropsis 2024-2025.

#include "AbilitySystem/ModMagCalc/MMC_PhysicalDefense.h"
#include "AbilitySystem/BaseAttributeSet.h"

UMMC_PhysicalDefense::UMMC_PhysicalDefense()
{
	VitalityDef.AttributeToCapture = UBaseAttributeSet::GetVitalityAttribute();
	VitalityDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	VitalityDef.bSnapshot = false;
	
	PhysicalDefenseBaseDef.AttributeToCapture = UBaseAttributeSet::GetPhysicalDefenseBaseAttribute();
	PhysicalDefenseBaseDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	PhysicalDefenseBaseDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(PhysicalDefenseBaseDef);
	RelevantAttributesToCapture.Add(VitalityDef);
}

float UMMC_PhysicalDefense::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	// Gather tags from source and target
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float Vitality = 0.f;
	GetCapturedAttributeMagnitude(VitalityDef, Spec, EvaluationParameters, Vitality);
	Vitality = FMath::Max<float>(Vitality, 0.f);

	float PhysicalDefenseBase = 0.f;
	GetCapturedAttributeMagnitude(PhysicalDefenseBaseDef, Spec, EvaluationParameters, PhysicalDefenseBase);
	PhysicalDefenseBase = FMath::Max<float>(PhysicalDefenseBase, 0.f);
		
	return (2.f + Vitality) * 0.25f + 6.f + PhysicalDefenseBase;
}
