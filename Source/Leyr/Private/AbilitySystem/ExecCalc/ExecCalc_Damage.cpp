// @ Retropsis 2024-2025.

#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/BaseAttributeSet.h"
#include "AbilitySystem/LeyrAbilitySystemLibrary.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "AI/AIData.h"
#include "Data/ItemData.h"
#include "Game/BaseGameplayTags.h"
#include "Interaction/AIInterface.h"
#include "Interaction/CombatInterface.h"
#include "Kismet/KismetMathLibrary.h"

struct BaseDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Strength);
	
	DECLARE_ATTRIBUTE_CAPTUREDEF(Health);
	DECLARE_ATTRIBUTE_CAPTUREDEF(MaxHealth);
	
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
	DECLARE_ATTRIBUTE_CAPTUREDEF(ExecutionResistance);

	BaseDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, Strength, Source, false);
		
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, Health, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, MaxHealth, Target, false);
		
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
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, ExecutionResistance, Target, false);
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
	
	RelevantAttributesToCapture.Add(GetDamageStatics().HealthDef);
	RelevantAttributesToCapture.Add(GetDamageStatics().MaxHealthDef);
	
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
	RelevantAttributesToCapture.Add(GetDamageStatics().ExecutionResistanceDef);
}

void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition> TagsToCaptureDefs;

	const FBaseGameplayTags& GameplayTags = FBaseGameplayTags::Get();
	TagsToCaptureDefs.Add(GameplayTags.Attributes_Primary_Strength, GetDamageStatics().StrengthDef);
	
	TagsToCaptureDefs.Add(GameplayTags.Attributes_Vital_Health, GetDamageStatics().HealthDef);
	TagsToCaptureDefs.Add(GameplayTags.Attributes_Secondary_MaxHealth, GetDamageStatics().MaxHealthDef);
	
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
	TagsToCaptureDefs.Add(GameplayTags.Attributes_Resistance_Execution, GetDamageStatics().ExecutionResistanceDef);
	
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
	// DetermineStatusEffect(ExecutionParams, Spec, EvaluationParameters, TagsToCaptureDefs);
	
	// Get Damage Set by Caller Magnitude
	float Damage = 0.f;
	for (const TTuple<FGameplayTag, FGameplayTag>& Pair : FBaseGameplayTags::Get().DamageTypesToResistances)
	{
		const FGameplayTag DamageTypeTag = Pair.Key;
		const FGameplayTag ResistanceTag = Pair.Value;
		
		checkf(TagsToCaptureDefs.Contains(ResistanceTag), TEXT("TagsToCaptureDefs doesn't contain Tag: [%s] in ExecCalc_Damage"), *ResistanceTag.ToString());
		const FGameplayEffectAttributeCaptureDefinition CaptureDef = TagsToCaptureDefs[ResistanceTag];

		float DamageTypeValue = Spec.GetSetByCallerMagnitude(Pair.Key, false);
		if(DamageTypeValue <= 0.f) continue;

		float Resistance = 0.f;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(CaptureDef, EvaluationParameters, Resistance);
		Resistance = FMath::Clamp(Resistance, 0.f, 100.f);

		DamageTypeValue *= ( 100.f - Resistance ) / 100.f;

		if (ULeyrAbilitySystemLibrary::IsRadialDamage(EffectContextHandle))
		{
			DamageTypeValue = ULeyrAbilitySystemLibrary::GetRadialDamageWithFalloff(
			   TargetAvatar, DamageTypeValue, 0.f,
			   ULeyrAbilitySystemLibrary::GetRadialDamageOrigin(EffectContextHandle),
			   ULeyrAbilitySystemLibrary::GetRadialDamageInnerRadius(EffectContextHandle),
			   ULeyrAbilitySystemLibrary::GetRadialDamageOuterRadius(EffectContextHandle),
			   1.f);
		}
		
		Damage += DamageTypeValue;
	}
	GEngine->AddOnScreenDebugMessage(123456, 30.f, FColor::Cyan, FString::Printf(TEXT("Raw Damage: %f"), Damage));

	float PhysicalAttackMastery = ApplyMasteryEffects(ExecutionParams);
	const FActiveGameplayEffectHandle ActiveSourceObjectEffectHandle = ApplyEquipmentEffects(ExecutionParams);
	
	float SourcePhysicalAttack = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDamageStatics().PhysicalAttackDef, EvaluationParameters, SourcePhysicalAttack);
	SourcePhysicalAttack = FMath::Max<float>(SourcePhysicalAttack, 0.f);
	
	const float EffectivePhysicalAttack = 1.f + FMath::Clamp(SourcePhysicalAttack, 0.f, 100.f) / 100.f;
	// const float EffectivePhysicalAttack = SourcePhysicalAttack *= ( 100 - SourceArmorPenetration * ArmorPenetrationCoefficient ) / 100.f;
	// const FRealCurve* EffectivePhysicalAttackCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(FName("EffectivePhysicalAttack"), FString());
	// const float EffectivePhysicalAttackCoefficient = EffectivePhysicalAttackCurve->Eval(TargetCharacterLevel);
	// GEngine->AddOnScreenDebugMessage(445577, 5.f, FColor::Cyan, FString::Printf(TEXT("EffectivePAtk: [%f]"), EffectivePhysicalAttack));
	Damage += Damage * EffectivePhysicalAttack * PhysicalAttackMastery;
	// GEngine->AddOnScreenDebugMessage(123457, 30.f, FColor::Cyan, FString::Printf(TEXT("Damage: %f including [P.Atk %f - P.Atk Mastery %f]"), Damage, SourcePhysicalAttack, PhysicalAttackMastery));
		
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
	Damage *= ( 100 - EffectivePhysicalDefense * EffectivePhysicalDefenseCoefficient ) / 100.f;
	// GEngine->AddOnScreenDebugMessage(123458, 30.f, FColor::Cyan, FString::Printf(TEXT("Damage reduction: %f [P.Def %f]"), EffectivePhysicalDefense * EffectivePhysicalDefenseCoefficient, EffectivePhysicalDefense));
	
	float SourceCriticalHitChance = 0.f;
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

	// Capture Execution Tag, then capture health and max health if needed
	// TODO: Not correct way to do this check
	float ExecutionDamageValue = Spec.GetSetByCallerMagnitude(GameplayTags.Damage_Execute, false);
	if (ExecutionDamageValue > 0.f)
	{
		ULeyrAbilitySystemLibrary::SetIsExecuteHit(EffectContextHandle, true);
		int32 StackCount = 0;
		
		FGameplayEffectQuery Query;
		Query.MakeQuery_MatchAnyEffectTags(GameplayTags.Indicator_Execute.GetSingleTagContainer());
		StackCount = TargetASC->GetAggregatedStackCount(Query);
		TargetASC->RemoveActiveEffectsWithGrantedTags(GameplayTags.Indicator_Execute.GetSingleTagContainer());
		
		float TargetHealth = 0.f;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDamageStatics().HealthDef, EvaluationParameters, TargetHealth);
		TargetHealth = FMath::Max<float>(TargetHealth, 0.f);
		
		float TargetMaxHealth = 0.f;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDamageStatics().MaxHealthDef, EvaluationParameters, TargetMaxHealth);
		TargetMaxHealth = FMath::Max<float>(TargetMaxHealth, 0.f);

		float Scaler = 0.f;
		float MaxScaler = .15f;
		switch (IAIInterface::Execute_GetEncounterSize(TargetAvatar))
		{
		case EEncounterSize::Critter: 
		case EEncounterSize::Default:
			break;
		case EEncounterSize::Humanoid:
			Scaler = .65f;
			MaxScaler = .65f;
			break;
		case EEncounterSize::Large:
			Scaler = .1f;
			MaxScaler = .2f;
			break;
		case EEncounterSize::Boss:
			Scaler = .03f;
			MaxScaler = .15f;
			break;
		}

		float PercentMissingHealth = UKismetMathLibrary::SafeDivide(TargetHealth, TargetMaxHealth);
		float CappedPercentMissingHealth = FMath::Min(StackCount * Scaler, MaxScaler);
		
		float MissingHealthBonusDamage = TargetMaxHealth * CappedPercentMissingHealth;
		Damage += MissingHealthBonusDamage;
	}

	// Double damage plus a bonus if critical hit
	Damage = bCriticalHit ? 2.f * Damage + SourceCriticalHitDamage : Damage;

	const FGameplayModifierEvaluatedData EvaluatedData(UBaseAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, Damage);
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
	
	if(ActiveSourceObjectEffectHandle.IsValid()) ExecutionParams.GetSourceAbilitySystemComponent()->RemoveActiveGameplayEffect(ActiveSourceObjectEffectHandle);
}

float UExecCalc_Damage::ApplyMasteryEffects(const FGameplayEffectCustomExecutionParameters& ExecutionParams) const
{
	const FBaseGameplayTags& GameplayTags = FBaseGameplayTags::Get();
	FGameplayTagContainer MasteryTags;
	MasteryTags.AddTag(GameplayTags.Abilities_Passive_Mastery);
	TArray<FGameplayAbilitySpec*> MasteryAbilities;
	ExecutionParams.GetSourceAbilitySystemComponent()->GetActivatableGameplayAbilitySpecsByAllMatchingTags(MasteryTags, MasteryAbilities);
	
	if (UObject* SourceObject = ExecutionParams.GetOwningSpec().GetContext().GetSourceObject())
	{
		if (const UItemData* Asset = Cast<UItemData>(SourceObject))
		{
			if (ExecutionParams.GetSourceAbilitySystemComponent()->HasMatchingGameplayTag(Asset->MasteryTag))
			{
				TArray<FGameplayEffectSpec> OutSpecCopies;
				ExecutionParams.GetSourceAbilitySystemComponent()->GetAllActiveGameplayEffectSpecs(OutSpecCopies);
				for (FGameplayEffectSpec& EffectSpec : OutSpecCopies)
				{
					if (EffectSpec.Def.Get()->GetGrantedTags().HasTagExact(Asset->MasteryTag))
					{
						return 1.f + FMath::Min(EffectSpec.GetLevel() * 0.05, 1.f);
					}
				}
			}			
		}
	}
	return 1.f;
}

FActiveGameplayEffectHandle UExecCalc_Damage::ApplyEquipmentEffects(const FGameplayEffectCustomExecutionParameters& ExecutionParams) const
{
	UObject* SourceObject = ExecutionParams.GetOwningSpec().GetContext().GetSourceObject();
	if(SourceObject == nullptr) return FActiveGameplayEffectHandle();
	
	FGameplayEffectContextHandle EffectContext = ExecutionParams.GetSourceAbilitySystemComponent()->MakeEffectContext();
	EffectContext.AddSourceObject(SourceObject);

	FString SourceObjectName = FString::Printf(TEXT("GE_SourceObjectEffects"));
	UGameplayEffect* Effect = NewObject<UGameplayEffect>(SourceObject, FName(SourceObjectName));

	Effect->DurationPolicy = EGameplayEffectDurationType::Infinite;
	
	// UTargetTagsGameplayEffectComponent& AssetTagsComponent = Effect->FindOrAddComponent<UTargetTagsGameplayEffectComponent>();
	// FInheritedTagContainer InheritedTagContainer;
	// InheritedTagContainer.Added.AddTag(EquipmentSlot);
	// AssetTagsComponent.SetAndApplyTargetTagChanges(InheritedTagContainer);
	
	if (const UItemData* Asset = Cast<UItemData>(SourceObject))
	{
		Effect->Modifiers.Append(Asset->Modifiers);
	}

	if (FGameplayEffectSpec* MutableSpec = new FGameplayEffectSpec(Effect, EffectContext, 1.f))
	{
		return ExecutionParams.GetSourceAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*MutableSpec, ExecutionParams.GetSourceAbilitySystemComponent());
	}
	return FActiveGameplayEffectHandle();
}