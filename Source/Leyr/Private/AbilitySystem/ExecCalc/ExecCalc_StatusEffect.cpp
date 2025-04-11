// @ Retropsis 2024-2025.

#include "AbilitySystem/ExecCalc/ExecCalc_StatusEffect.h"
#include "AbilitySystem/BaseAttributeSet.h"
#include "AbilitySystem/LeyrAbilitySystemLibrary.h"
#include "Game/BaseGameplayTags.h"
#include "Interaction/CombatInterface.h"

struct BaseStatusEffectStatics
{	
	DECLARE_ATTRIBUTE_CAPTUREDEF(Health);
	DECLARE_ATTRIBUTE_CAPTUREDEF(MaxHealth);
	DECLARE_ATTRIBUTE_CAPTUREDEF(MagicalAttack);
	DECLARE_ATTRIBUTE_CAPTUREDEF(MagicalDefense);
	DECLARE_ATTRIBUTE_CAPTUREDEF(BluntResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CuttingResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(PiercingResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(FireResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(IceResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(WindResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(LightningResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(HolyResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(DarkResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(NoxiousResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ExecutionResistance);

	BaseStatusEffectStatics()
	{		
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, Health, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, MaxHealth, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, MagicalAttack, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, MagicalDefense, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, BluntResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, CuttingResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, PiercingResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, FireResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, IceResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, WindResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, LightningResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, HolyResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, DarkResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, NoxiousResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, ExecutionResistance, Target, false);
	}
};

static const BaseStatusEffectStatics& GetStatusEffectStatics()
{
	static BaseStatusEffectStatics SEStatics;
	return SEStatics;
}

UExecCalc_StatusEffect::UExecCalc_StatusEffect()
{	
	RelevantAttributesToCapture.Add(GetStatusEffectStatics().HealthDef);
	RelevantAttributesToCapture.Add(GetStatusEffectStatics().MaxHealthDef);
	RelevantAttributesToCapture.Add(GetStatusEffectStatics().MagicalAttackDef);
	RelevantAttributesToCapture.Add(GetStatusEffectStatics().MagicalDefenseDef);
	RelevantAttributesToCapture.Add(GetStatusEffectStatics().BluntResistanceDef);
	RelevantAttributesToCapture.Add(GetStatusEffectStatics().CuttingResistanceDef);
	RelevantAttributesToCapture.Add(GetStatusEffectStatics().PiercingResistanceDef);
	RelevantAttributesToCapture.Add(GetStatusEffectStatics().FireResistanceDef);
	RelevantAttributesToCapture.Add(GetStatusEffectStatics().IceResistanceDef);
	RelevantAttributesToCapture.Add(GetStatusEffectStatics().WindResistanceDef);
	RelevantAttributesToCapture.Add(GetStatusEffectStatics().LightningResistanceDef);
	RelevantAttributesToCapture.Add(GetStatusEffectStatics().HolyResistanceDef);
	RelevantAttributesToCapture.Add(GetStatusEffectStatics().DarkResistanceDef);
	RelevantAttributesToCapture.Add(GetStatusEffectStatics().NoxiousResistanceDef);
	RelevantAttributesToCapture.Add(GetStatusEffectStatics().ExecutionResistanceDef);
}

void UExecCalc_StatusEffect::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{	
	TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition> TagsToCaptureDefs;

	const FBaseGameplayTags& GameplayTags = FBaseGameplayTags::Get();
	
	TagsToCaptureDefs.Add(GameplayTags.Attributes_Vital_Health, GetStatusEffectStatics().HealthDef);
	TagsToCaptureDefs.Add(GameplayTags.Attributes_Secondary_MaxHealth, GetStatusEffectStatics().MaxHealthDef);
	TagsToCaptureDefs.Add(GameplayTags.Attributes_Secondary_MagicalAttack, GetStatusEffectStatics().MagicalAttackDef);
	TagsToCaptureDefs.Add(GameplayTags.Attributes_Secondary_MagicalDefense, GetStatusEffectStatics().MagicalDefenseDef);
	TagsToCaptureDefs.Add(GameplayTags.Attributes_Resistance_Blunt, GetStatusEffectStatics().BluntResistanceDef);
	TagsToCaptureDefs.Add(GameplayTags.Attributes_Resistance_Cutting, GetStatusEffectStatics().CuttingResistanceDef);
	TagsToCaptureDefs.Add(GameplayTags.Attributes_Resistance_Piercing, GetStatusEffectStatics().PiercingResistanceDef);
	TagsToCaptureDefs.Add(GameplayTags.Attributes_Resistance_Fire, GetStatusEffectStatics().FireResistanceDef);
	TagsToCaptureDefs.Add(GameplayTags.Attributes_Resistance_Ice, GetStatusEffectStatics().IceResistanceDef);
	TagsToCaptureDefs.Add(GameplayTags.Attributes_Resistance_Wind, GetStatusEffectStatics().WindResistanceDef);
	TagsToCaptureDefs.Add(GameplayTags.Attributes_Resistance_Lightning, GetStatusEffectStatics().LightningResistanceDef);
	TagsToCaptureDefs.Add(GameplayTags.Attributes_Resistance_Holy, GetStatusEffectStatics().HolyResistanceDef);
	TagsToCaptureDefs.Add(GameplayTags.Attributes_Resistance_Dark, GetStatusEffectStatics().DarkResistanceDef);
	TagsToCaptureDefs.Add(GameplayTags.Attributes_Resistance_Noxious, GetStatusEffectStatics().NoxiousResistanceDef);
	TagsToCaptureDefs.Add(GameplayTags.Attributes_Resistance_Execution, GetStatusEffectStatics().ExecutionResistanceDef);
	
	const UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();

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
	FGameplayEffectContextHandle EffectContextHandle = Spec.GetContext();
	
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;
	
	// Status Effect
	DetermineStatusEffect(ExecutionParams, Spec, EvaluationParameters, TagsToCaptureDefs);

	const FGameplayModifierEvaluatedData EvaluatedData(UBaseAttributeSet::GetIncomingStatusEffectAttribute(), EGameplayModOp::Additive, 1.f /*  Status Effect Magnitude */ );
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
}

void UExecCalc_StatusEffect::DetermineStatusEffect(const FGameplayEffectCustomExecutionParameters& ExecutionParams, const FGameplayEffectSpec& Spec, const FAggregatorEvaluateParameters& EvaluationParameters,
	const TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition>& InTagsToDefs) const
{
	const FBaseGameplayTags& GameplayTags = FBaseGameplayTags::Get();
	for (TTuple<FGameplayTag, FGameplayTag> Pair : GameplayTags.StatusEffectsToDamageTypes)
	{	
		const FGameplayTag& StatusEffectType = Pair.Key;
		if (Spec.GetSetByCallerMagnitude(StatusEffectType, false, -1.f) > -.5f) // .5 padding for floating point [im]precision
		{
			// Determine if there was a successful status effect
			const float SourceStatusEffectChance = Spec.GetSetByCallerMagnitude(GameplayTags.StatusEffect_Chance, false, -1.f);

			float TargetStatusEffectResistance = 0.f;
			const FGameplayTag& ResistanceTag = GameplayTags.StatusEffectsToResistances[StatusEffectType];
			ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(InTagsToDefs[ResistanceTag], EvaluationParameters, TargetStatusEffectResistance);
			TargetStatusEffectResistance = FMath::Max<float>(TargetStatusEffectResistance, 0.f);
			const float EffectiveStatusEffectChance = SourceStatusEffectChance * ( 100 - TargetStatusEffectResistance ) / 100.f;
			
			if (FMath::RandRange(1, 100) < EffectiveStatusEffectChance)
			{
				FGameplayEffectContextHandle ContextHandle = Spec.GetContext();

				ULeyrAbilitySystemLibrary::SetIsSuccessfulStatusEffect(ContextHandle, true);
				const float StatusEffectDuration = Spec.GetSetByCallerMagnitude(GameplayTags.StatusEffect_Duration, false, -1.f);
				ULeyrAbilitySystemLibrary::SetStatusEffectDuration(ContextHandle, StatusEffectDuration);

				if (GameplayTags.StatusEffectsToDamageTypes.Contains(Pair.Key) && GameplayTags.StatusEffectsToDamageTypes[Pair.Key].IsValid())
				{
					const FGameplayTag& StatusEffectDamageType = GameplayTags.StatusEffectsToDamageTypes[Pair.Key];
					ULeyrAbilitySystemLibrary::SetDamageType(ContextHandle, StatusEffectDamageType);
					ULeyrAbilitySystemLibrary::SetStatusEffectType(ContextHandle, StatusEffectType);

					const float StatusEffectDamage = Spec.GetSetByCallerMagnitude(GameplayTags.StatusEffect_Damage, false, -1.f);
					const float StatusEffectFrequency = Spec.GetSetByCallerMagnitude(GameplayTags.StatusEffect_Frequency, false, -1.f);

					ULeyrAbilitySystemLibrary::SetStatusEffectDamage(ContextHandle, StatusEffectDamage);
					ULeyrAbilitySystemLibrary::SetStatusEffectFrequency(ContextHandle, StatusEffectFrequency);
				}
			}
		}
	}
}