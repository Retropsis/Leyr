// @ Retropsis 2024-2025.

#include "AbilitySystem/ExecCalc/ExecCalc_Healing.h"
#include "AbilitySystem/BaseAttributeSet.h"
#include "AbilitySystem/LeyrAbilitySystemLibrary.h"
#include "Game/BaseGameplayTags.h"
#include "Interaction/CombatInterface.h"
#include "Kismet/KismetMathLibrary.h"

struct BaseHealingStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Wisdom);
	
	DECLARE_ATTRIBUTE_CAPTUREDEF(Health);
	DECLARE_ATTRIBUTE_CAPTUREDEF(MaxHealth);

	BaseHealingStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, Wisdom, Source, false);
		
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, Health, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, MaxHealth, Target, false);
	}
};

static const BaseHealingStatics& GetHealingStatics()
{
	static BaseHealingStatics HStatics;
	return HStatics;
}

UExecCalc_Healing::UExecCalc_Healing()
{
	RelevantAttributesToCapture.Add(GetHealingStatics().WisdomDef);
	
	RelevantAttributesToCapture.Add(GetHealingStatics().HealthDef);
	RelevantAttributesToCapture.Add(GetHealingStatics().MaxHealthDef);
}

void UExecCalc_Healing::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition> TagsToCaptureDefs;

	const FBaseGameplayTags& GameplayTags = FBaseGameplayTags::Get();
	TagsToCaptureDefs.Add(GameplayTags.Attributes_Primary_Strength, GetHealingStatics().WisdomDef);
	
	TagsToCaptureDefs.Add(GameplayTags.Attributes_Vital_Health, GetHealingStatics().HealthDef);
	TagsToCaptureDefs.Add(GameplayTags.Attributes_Secondary_MaxHealth, GetHealingStatics().MaxHealthDef);
	
	const UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();

	AActor* SourceAvatar = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
	AActor* TargetAvatar = TargetASC ? TargetASC->GetAvatarActor() : nullptr;
	int32 SourceCharacterLevel = 1;
	if (SourceAvatar->Implements<UCombatInterface>())
	{
		SourceCharacterLevel = ICombatInterface::Execute_GetCharacterLevel(SourceAvatar);
	}
	int32 TargetCharacterLevel = 1;
	if (TargetAvatar->Implements<UCombatInterface>())
	{
		TargetCharacterLevel = ICombatInterface::Execute_GetCharacterLevel(TargetAvatar);
	}

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;
	
	// Get Damage Set by Caller Magnitude
	float HealingAmount = Spec.GetSetByCallerMagnitude(FBaseGameplayTags::Get().Healing_Health, false);

	float SourceWisdom = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetHealingStatics().WisdomDef, EvaluationParameters, SourceWisdom);
	SourceWisdom = FMath::Max<float>(SourceWisdom, 0.f);
	
	float TargetHealth = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetHealingStatics().HealthDef, EvaluationParameters, TargetHealth);
	TargetHealth = FMath::Max<float>(TargetHealth, 0.f);

	float TargetMaxHealth = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetHealingStatics().MaxHealthDef, EvaluationParameters, TargetMaxHealth);
	TargetMaxHealth = FMath::Max<float>(TargetMaxHealth, 0.f);

	const FGameplayModifierEvaluatedData EvaluatedData(UBaseAttributeSet::GetIncomingHealingAttribute(), EGameplayModOp::Additive, HealingAmount);
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
}
