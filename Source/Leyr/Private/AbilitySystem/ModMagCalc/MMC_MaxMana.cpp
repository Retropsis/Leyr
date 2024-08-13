// @ Retropsis 2024-2025.

#include "AbilitySystem/ModMagCalc/MMC_MaxMana.h"
#include "AbilitySystem/BaseAttributeSet.h"
#include "Interaction/CombatInterface.h"

UMMC_MaxMana::UMMC_MaxMana()
{
	IntelligenceDef.AttributeToCapture = UBaseAttributeSet::GetIntelligenceAttribute();
	IntelligenceDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	IntelligenceDef.bSnapshot = false;
	
	WisdomDef.AttributeToCapture = UBaseAttributeSet::GetIntelligenceAttribute();
	WisdomDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	WisdomDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(IntelligenceDef);
	RelevantAttributesToCapture.Add(WisdomDef);
}

float UMMC_MaxMana::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
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
	
	float Wisdom = 0.f;
	GetCapturedAttributeMagnitude(WisdomDef, Spec, EvaluationParameters, Wisdom);
	Wisdom = FMath::Max<float>(Wisdom, 0.f);

	ICombatInterface* CombatInterface = Cast<ICombatInterface>(Spec.GetContext().GetSourceObject());
	const int32 CharacterLevel = CombatInterface->GetCharacterLevel();

	return 50.f + 2.5f * (Intelligence * 0.25 + Wisdom * 0.75) + 15.f * CharacterLevel;
}
