// @ Retropsis 2024-2025.

#include "AbilitySystem/ModMagCalc/MMC_PhysicalAttack.h"
#include "AbilitySystem/BaseAttributeSet.h"

UMMC_PhysicalAttack::UMMC_PhysicalAttack()
{
	StrengthDef.AttributeToCapture = UBaseAttributeSet::GetStrengthAttribute();
	StrengthDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	StrengthDef.bSnapshot = false;
	
	PhysicalAttackBaseDef.AttributeToCapture = UBaseAttributeSet::GetPhysicalAttackBaseAttribute();
	PhysicalAttackBaseDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	PhysicalAttackBaseDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(PhysicalAttackBaseDef);
	RelevantAttributesToCapture.Add(StrengthDef);
}

float UMMC_PhysicalAttack::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	// Gather tags from source and target
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float Strength = 0.f;
	GetCapturedAttributeMagnitude(StrengthDef, Spec, EvaluationParameters, Strength);
	Strength = FMath::Max<float>(Strength, 0.f);

	float PhysicalAttackBase = 0.f;
	GetCapturedAttributeMagnitude(PhysicalAttackBaseDef, Spec, EvaluationParameters, PhysicalAttackBase);
	PhysicalAttackBase = FMath::Max<float>(PhysicalAttackBase, 0.f);
		
	return (2.f + Strength) * 0.25f + 6.f + PhysicalAttackBase;
}
