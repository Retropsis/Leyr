// @ Retropsis 2024-2025.

#include "AbilitySystem/ModMagCalc/MMC_Damage.h"
#include "AbilitySystem/BaseAttributeSet.h"
#include "GameFramework/Character.h"
#include "Player/PlayerCharacterController.h"

UMMC_Damage::UMMC_Damage()
{
	HealthDef.AttributeToCapture = UBaseAttributeSet::GetHealthAttribute();
	HealthDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	HealthDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(HealthDef);
}

float UMMC_Damage::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	// Gather tags from source and target
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;
	
	float Health = 0.f;
	GetCapturedAttributeMagnitude(HealthDef, Spec, EvaluationParameters, Health);
	Health = FMath::Max<float>(Health, 0.f);

	float Damage = 0.f;
	

	if(ACharacter* CharacterInstigator = Cast<ACharacter>(Spec.GetEffectContext().GetEffectCauser()))
	{
		ShowFloatingText(CharacterInstigator, Damage, false, false);
	}
	
	return FMath::Max<float>(Health - Damage, 0.f);
}

void UMMC_Damage::ShowFloatingText(ACharacter* CharacterInstigator, float Damage, bool bBlockedHit, bool bCriticalHit) const
{
	if(APlayerCharacterController* PC = Cast<APlayerCharacterController>(CharacterInstigator->Controller))
	{
		const FUIMessageData MessageData{ EMessageType::DamageToPlayer, Damage, CharacterInstigator, bBlockedHit, bCriticalHit };
		PC->ClientShowDamageNumber(MessageData);
	}
}