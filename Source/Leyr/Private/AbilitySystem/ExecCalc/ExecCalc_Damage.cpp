// @ Retropsis 2024-2025.

#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/BaseAttributeSet.h"
#include "AbilitySystem/LeyrAbilitySystemLibrary.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Game/BaseGameplayTags.h"
#include "Interaction/CombatInterface.h"

struct BaseDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Strength);
	
	DECLARE_ATTRIBUTE_CAPTUREDEF(PhysicalAttack);
	DECLARE_ATTRIBUTE_CAPTUREDEF(PhysicalDefense);
	
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArmorPenetration);
	DECLARE_ATTRIBUTE_CAPTUREDEF(BlockChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitDamage);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitResistance);
	
	DECLARE_ATTRIBUTE_CAPTUREDEF(PhysicalResistance);
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

	BaseDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, Strength, Source, false);
		
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, PhysicalAttack, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, PhysicalDefense, Target, false);
		
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, ArmorPenetration, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, BlockChance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, CriticalHitChance, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, CriticalHitDamage, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, CriticalHitResistance, Target, false);
		
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, PhysicalResistance, Target, false);
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
	}
};

static const BaseDamageStatics& GetDamageStatics()
{
	static BaseDamageStatics DStatics;
	return DStatics;
}

UExecCalc_Damage::UExecCalc_Damage()
{
	RelevantAttributesToCapture.Add(GetDamageStatics().StrengthDef);
	
	RelevantAttributesToCapture.Add(GetDamageStatics().PhysicalAttackDef);
	RelevantAttributesToCapture.Add(GetDamageStatics().PhysicalDefenseDef);
	
	RelevantAttributesToCapture.Add(GetDamageStatics().ArmorPenetrationDef);
	RelevantAttributesToCapture.Add(GetDamageStatics().BlockChanceDef);
	RelevantAttributesToCapture.Add(GetDamageStatics().CriticalHitChanceDef);
	RelevantAttributesToCapture.Add(GetDamageStatics().CriticalHitDamageDef);
	RelevantAttributesToCapture.Add(GetDamageStatics().CriticalHitResistanceDef);
	
	RelevantAttributesToCapture.Add(GetDamageStatics().PhysicalResistanceDef);
	RelevantAttributesToCapture.Add(GetDamageStatics().BluntResistanceDef);
	RelevantAttributesToCapture.Add(GetDamageStatics().CuttingResistanceDef);
	RelevantAttributesToCapture.Add(GetDamageStatics().PiercingResistanceDef);
	RelevantAttributesToCapture.Add(GetDamageStatics().FireResistanceDef);
	RelevantAttributesToCapture.Add(GetDamageStatics().IceResistanceDef);
	RelevantAttributesToCapture.Add(GetDamageStatics().WindResistanceDef);
	RelevantAttributesToCapture.Add(GetDamageStatics().LightningResistanceDef);
	RelevantAttributesToCapture.Add(GetDamageStatics().HolyResistanceDef);
	RelevantAttributesToCapture.Add(GetDamageStatics().DarkResistanceDef);
	RelevantAttributesToCapture.Add(GetDamageStatics().NoxiousResistanceDef);
}

void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition> TagsToCaptureDefs;

	const FBaseGameplayTags& GameplayTags = FBaseGameplayTags::Get();
	TagsToCaptureDefs.Add(GameplayTags.Attributes_Primary_Strength, GetDamageStatics().StrengthDef);
	
	TagsToCaptureDefs.Add(GameplayTags.Attributes_Secondary_PhysicalAttack, GetDamageStatics().PhysicalAttackDef);
	TagsToCaptureDefs.Add(GameplayTags.Attributes_Secondary_PhysicalDefense, GetDamageStatics().PhysicalDefenseDef);
	
	TagsToCaptureDefs.Add(GameplayTags.Attributes_Secondary_ArmorPenetration, GetDamageStatics().ArmorPenetrationDef);
	TagsToCaptureDefs.Add(GameplayTags.Attributes_Secondary_BlockChance, GetDamageStatics().BlockChanceDef);
	TagsToCaptureDefs.Add(GameplayTags.Attributes_Secondary_CriticalHitChance, GetDamageStatics().CriticalHitChanceDef);
	TagsToCaptureDefs.Add(GameplayTags.Attributes_Secondary_CriticalHitDamage, GetDamageStatics().CriticalHitDamageDef);
	TagsToCaptureDefs.Add(GameplayTags.Attributes_Secondary_CriticalHitResistance, GetDamageStatics().CriticalHitResistanceDef);
		
	TagsToCaptureDefs.Add(GameplayTags.Attributes_Resistance_Physical, GetDamageStatics().PhysicalResistanceDef);
	TagsToCaptureDefs.Add(GameplayTags.Attributes_Resistance_Blunt, GetDamageStatics().BluntResistanceDef);
	TagsToCaptureDefs.Add(GameplayTags.Attributes_Resistance_Cutting, GetDamageStatics().CuttingResistanceDef);
	TagsToCaptureDefs.Add(GameplayTags.Attributes_Resistance_Piercing, GetDamageStatics().PiercingResistanceDef);
	TagsToCaptureDefs.Add(GameplayTags.Attributes_Resistance_Fire, GetDamageStatics().FireResistanceDef);
	TagsToCaptureDefs.Add(GameplayTags.Attributes_Resistance_Ice, GetDamageStatics().IceResistanceDef);
	TagsToCaptureDefs.Add(GameplayTags.Attributes_Resistance_Wind, GetDamageStatics().WindResistanceDef);
	TagsToCaptureDefs.Add(GameplayTags.Attributes_Resistance_Lightning, GetDamageStatics().LightningResistanceDef);
	TagsToCaptureDefs.Add(GameplayTags.Attributes_Resistance_Holy, GetDamageStatics().HolyResistanceDef);
	TagsToCaptureDefs.Add(GameplayTags.Attributes_Resistance_Dark, GetDamageStatics().DarkResistanceDef);
	TagsToCaptureDefs.Add(GameplayTags.Attributes_Resistance_Noxious, GetDamageStatics().NoxiousResistanceDef);
	
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
	
	// Status Effect
	DetermineStatusEffect(ExecutionParams, Spec, EvaluationParameters, TagsToCaptureDefs);
	
	FGameplayEffectContextHandle EffectContextHandle = Spec.GetContext();
	
	// Get Damage Set by Caller Magnitude
	float Damage = 0.f;
	for (const TTuple<FGameplayTag, FGameplayTag>& Pair : FBaseGameplayTags::Get().DamageTypesToResistances)
	{
		const FGameplayTag DamageTypeTag = Pair.Key;
		const FGameplayTag ResistanceTag = Pair.Value;

		if(DamageTypeTag.MatchesTagExact(FBaseGameplayTags::Get().Damage_Execute))
		{
			float DamageTypeValue = Spec.GetSetByCallerMagnitude(Pair.Key, false);
			if(DamageTypeValue <= 0.f) continue;
			ULeyrAbilitySystemLibrary::SetIsExecuteHit(EffectContextHandle, true);
			continue;
		}
		checkf(TagsToCaptureDefs.Contains(ResistanceTag), TEXT("TagsToCaptureDefs doesn't contain Tag: [%s] in ExecCalc_Damage"), *ResistanceTag.ToString());
		const FGameplayEffectAttributeCaptureDefinition CaptureDef = TagsToCaptureDefs[ResistanceTag];

		float DamageTypeValue = Spec.GetSetByCallerMagnitude(Pair.Key, false);
		if(DamageTypeValue <= 0.f) continue;

		float Resistance = 0.f;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(CaptureDef, EvaluationParameters, Resistance);
		Resistance = FMath::Clamp(Resistance, 0.f, 100.f);

		DamageTypeValue *= ( 100.f - Resistance ) / 100.f;
		Damage += DamageTypeValue;
	}
	
	// Capture BlockChance on Target, and determine if there was a successful Block
	float TargetBlockChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDamageStatics().BlockChanceDef, EvaluationParameters, TargetBlockChance);
	TargetBlockChance = FMath::Max<float>(TargetBlockChance, 0.f);

	const bool bBlocked = FMath::RandRange(1, 100) < TargetBlockChance;
	Damage = bBlocked ? Damage / 2.f : Damage;

	ULeyrAbilitySystemLibrary::SetIsBlockedHit(EffectContextHandle, bBlocked);

	float TargetPhysicalDefense = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDamageStatics().PhysicalDefenseDef, EvaluationParameters, TargetPhysicalDefense);
	TargetPhysicalDefense = FMath::Max<float>(TargetPhysicalDefense, 0.f);

	float SourceArmorPenetration = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDamageStatics().ArmorPenetrationDef, EvaluationParameters, SourceArmorPenetration);
	SourceArmorPenetration = FMath::Max<float>(SourceArmorPenetration, 0.f);
	
	const UCharacterClassInfo* CharacterClassInfo = ULeyrAbilitySystemLibrary::GetCharacterClassInfo(SourceAvatar);
	const FRealCurve* ArmorPenetrationCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(FName("ArmorPenetration"), FString());
	const float ArmorPenetrationCoefficient = ArmorPenetrationCurve->Eval(SourceCharacterLevel);

	// ArmorPenetration ignores a percentage of the Target's Armor.	
	const float EffectivePhysicalDefense = TargetPhysicalDefense *= ( 100 - SourceArmorPenetration * ArmorPenetrationCoefficient ) / 100.f;

	const FRealCurve* EffectivePhysicalDefenseCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(FName("EffectivePhysicalDefense"), FString());
	const float EffectivePhysicalDefenseCoefficient = EffectivePhysicalDefenseCurve->Eval(TargetCharacterLevel);
	// Armor ignores a percentage of incoming Damage.
	Damage *= ( 100 - EffectivePhysicalDefense * EffectivePhysicalDefenseCoefficient ) / 100.f;	float SourceCriticalHitChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDamageStatics().CriticalHitChanceDef, EvaluationParameters, SourceCriticalHitChance);
	SourceCriticalHitChance = FMath::Max<float>(SourceCriticalHitChance, 0.f);

	float TargetCriticalHitResistance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDamageStatics().CriticalHitResistanceDef, EvaluationParameters, TargetCriticalHitResistance);
	TargetCriticalHitResistance = FMath::Max<float>(TargetCriticalHitResistance, 0.f);

	float SourceCriticalHitDamage = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDamageStatics().CriticalHitDamageDef, EvaluationParameters, SourceCriticalHitDamage);
	SourceCriticalHitDamage = FMath::Max<float>(SourceCriticalHitDamage, 0.f);

	const FRealCurve* CriticalHitResistanceCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(FName("CriticalHitResistance"), FString());
	const float CriticalHitResistanceCoefficient = CriticalHitResistanceCurve->Eval(TargetCharacterLevel);

	// Critical Hit Resistance reduces Critical Hit Chance by a certain percentage
	const float EffectiveCriticalHitChance = SourceCriticalHitChance - TargetCriticalHitResistance * CriticalHitResistanceCoefficient;
	const bool bCriticalHit = FMath::RandRange(1, 100) < EffectiveCriticalHitChance;
	
	ULeyrAbilitySystemLibrary::SetIsCriticalHit(EffectContextHandle, bCriticalHit);

	// Double damage plus a bonus if critical hit
	Damage = bCriticalHit ? 2.f * Damage + SourceCriticalHitDamage : Damage;

	const FGameplayModifierEvaluatedData EvaluatedData(UBaseAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, Damage);
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
}

void UExecCalc_Damage::DetermineStatusEffect(const FGameplayEffectCustomExecutionParameters& ExecutionParams, const FGameplayEffectSpec& Spec, FAggregatorEvaluateParameters EvaluationParameters,
	const TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition>& InTagsToDefs) const
{
	const FBaseGameplayTags& GameplayTags = FBaseGameplayTags::Get();

	for (TTuple<FGameplayTag, FGameplayTag> Pair : GameplayTags.DamageTypesToStatusEffects)
	{
		const FGameplayTag& DamageType = Pair.Key;
		const FGameplayTag& StatusEffectType = Pair.Value;
		const float TypeDamage = Spec.GetSetByCallerMagnitude(DamageType, false, -1.f);
		if (TypeDamage > -.5f) // .5 padding for floating point [im]precision
		{
			// Determine if there was a successful debuff
			const float SourceStatusEffectChance = Spec.GetSetByCallerMagnitude(GameplayTags.StatusEffect_Chance, false, -1.f);

			float TargetStatusEffectResistance = 0.f;
			const FGameplayTag& ResistanceTag = GameplayTags.DamageTypesToResistances[DamageType];
			ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(InTagsToDefs[ResistanceTag], EvaluationParameters, TargetStatusEffectResistance);
			TargetStatusEffectResistance = FMath::Max<float>(TargetStatusEffectResistance, 0.f);
			const float EffectiveStatusEffectChance = SourceStatusEffectChance * ( 100 - TargetStatusEffectResistance ) / 100.f;
			
			if (FMath::RandRange(1, 100) < EffectiveStatusEffectChance)
			{
				FGameplayEffectContextHandle ContextHandle = Spec.GetContext();

				ULeyrAbilitySystemLibrary::SetIsSuccessfulStatusEffect(ContextHandle, true);
				ULeyrAbilitySystemLibrary::SetDamageType(ContextHandle, DamageType);

				const float StatusEffectDamage = Spec.GetSetByCallerMagnitude(GameplayTags.StatusEffect_Damage, false, -1.f);
				const float StatusEffectDuration = Spec.GetSetByCallerMagnitude(GameplayTags.StatusEffect_Duration, false, -1.f);
				const float StatusEffectFrequency = Spec.GetSetByCallerMagnitude(GameplayTags.StatusEffect_Frequency, false, -1.f);

				ULeyrAbilitySystemLibrary::SetStatusEffectDamage(ContextHandle, StatusEffectDamage);
				ULeyrAbilitySystemLibrary::SetStatusEffectDuration(ContextHandle, StatusEffectDuration);
				ULeyrAbilitySystemLibrary::SetStatusEffectFrequency(ContextHandle, StatusEffectFrequency);
			}
		}
	}
}