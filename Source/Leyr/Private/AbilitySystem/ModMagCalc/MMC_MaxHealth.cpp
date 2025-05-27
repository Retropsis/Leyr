// @ Retropsis 2024-2025.

#include "AbilitySystem/ModMagCalc/MMC_MaxHealth.h"
#include "AbilitySystem/BaseAttributeSet.h"
#include "Interaction/CombatInterface.h"

UMMC_MaxHealth::UMMC_MaxHealth()
{
	VitalityDef.AttributeToCapture = UBaseAttributeSet::GetVitalityAttribute();
	VitalityDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	VitalityDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(VitalityDef);
}

float UMMC_MaxHealth::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
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

	int32 CharacterLevel = 1;
	if (Spec.GetContext().GetSourceObject()->Implements<UCombatInterface>())
	{
		CharacterLevel = ICombatInterface::Execute_GetCharacterLevel(Spec.GetContext().GetSourceObject());
	}
	// return /*80.f + */2.5f * Vitality + 10.f * CharacterLevel;
	// GEngine->AddOnScreenDebugMessage(-1, 999.f, FColor::Magenta, FString::Printf(TEXT("[%s] Vitality[%f] CharacterLevel[%d] = [%f]"), *Spec.GetContext().GetSourceObject()->GetName(), Vitality, CharacterLevel, Vitality * CharacterLevel * 10.f));
	return Vitality * CharacterLevel * 10.f;
	/*
	 * Less than 10HP = (1 * 1 + 5 * 1 = 6) 10HP
	 * 100HP, 10 Vit, Level 1 (Level 1 * 10 Vit * 10)
	 * 2000HP = 20 Vit, Level 5 (Level 5 * 20 Vit * 10) + Post 1000
	 */
}
