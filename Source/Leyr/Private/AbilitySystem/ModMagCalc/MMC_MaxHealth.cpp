// @ Retropsis 2024-2025.

#include "AbilitySystem/ModMagCalc/MMC_MaxHealth.h"
#include "AbilitySystem/BaseAttributeSet.h"
#include "AI/AIData.h"
#include "Interaction/AIInterface.h"
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
	float HasFailed = GetCapturedAttributeMagnitude(VitalityDef, Spec, EvaluationParameters, Vitality);
	Vitality = FMath::Max<float>(Vitality, 0.f);

	// if (HasFailed)
	// {
	// 	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Magenta, FString::Printf(TEXT("%s has failed"), *Spec.GetContext().GetSourceObject()->GetName()));
	// }

	int32 CharacterLevel = 1;
	if (Spec.GetContext().GetSourceObject()->Implements<UCombatInterface>())
	{
		CharacterLevel = ICombatInterface::Execute_GetCharacterLevel(Spec.GetContext().GetSourceObject());
	}
	float EncounterSizeFactor = 1.f;
	if (Spec.GetContext().GetSourceObject()->Implements<UAIInterface>())
	{
		switch (IAIInterface::Execute_GetEncounterSize(Spec.GetContext().GetSourceObject()))
		{
		case EEncounterSize::Default: EncounterSizeFactor = 2.f;
			break;
		case EEncounterSize::Critter: EncounterSizeFactor = 1.f;
			break;
		case EEncounterSize::Humanoid: EncounterSizeFactor = 2.f;
			break;
		case EEncounterSize::Large: EncounterSizeFactor = 3.f;
			break;
		case EEncounterSize::Boss: EncounterSizeFactor = 20.f;
			break;
		}
	}
	// return /*80.f + */2.5f * Vitality + 10.f * CharacterLevel;
	// GEngine->AddOnScreenDebugMessage(-1, 999.f, FColor::Magenta, FString::Printf(TEXT("EncounterSizeFactor[%f] Vitality[%f] CharacterLevel[%d] = [%f]"), EncounterSizeFactor, Vitality, CharacterLevel, EncounterSizeFactor * Vitality + 5.f * CharacterLevel));
	return EncounterSizeFactor * Vitality + 5.f * CharacterLevel;
}
