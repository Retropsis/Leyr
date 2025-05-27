// @ Retropsis 2024-2025.

#include "Data/AttributeData.h"
#include "AbilitySystemComponent.h"
#include "AbilityTypes.h"
#include "AbilitySystem/BaseAttributeSet.h"

#define ATTRIBUTE(Name, GameplayAttribute, AttributeData) \
FGameplayModifierInfo ModifierInfo##Name; \
ModifierInfo##Name.Attribute = GameplayAttribute; \
ModifierInfo##Name.ModifierOp = EGameplayModOp::Override; \
ModifierInfo##Name.ModifierMagnitude = FScalableFloat(AttributeData.Value); \
Attributes.Add(ModifierInfo##Name); \

#define VITAL_ATTRIBUTE(Name, GameplayAttribute, AttributeData) \
FGameplayModifierInfo ModifierInfo##Name; \
ModifierInfo##Name.Attribute = GameplayAttribute; \
ModifierInfo##Name.ModifierOp = EGameplayModOp::Override; \
FCustomCalculationBasedFloat CustomCalculationBasedFloat##Name; \
CustomCalculationBasedFloat##Name.Coefficient = AttributeData.Coefficient; \
CustomCalculationBasedFloat##Name.PreMultiplyAdditiveValue = AttributeData.PreMultiplyAdditiveValue; \
CustomCalculationBasedFloat##Name.PostMultiplyAdditiveValue = AttributeData.PostMultiplyAdditiveValue; \
CustomCalculationBasedFloat##Name.CalculationClassMagnitude = AttributeData.CalculationClassMagnitude; \
ModifierInfo##Name.ModifierMagnitude = CustomCalculationBasedFloat##Name; \
VitalAttributes.Add(ModifierInfo##Name); \

TArray<FGameplayModifierInfo> UAttributeData::BuildPrimaryAttributes()
{
	TArray<FGameplayModifierInfo> Attributes;
	
	ATTRIBUTE(STR, UBaseAttributeSet::GetStrengthAttribute(), Strength);
	ATTRIBUTE(DEX, UBaseAttributeSet::GetDexterityAttribute(), Dexterity);
	ATTRIBUTE(VIT, UBaseAttributeSet::GetVitalityAttribute(), Vitality);
	ATTRIBUTE(INT, UBaseAttributeSet::GetIntelligenceAttribute(), Intelligence);
	ATTRIBUTE(WIS, UBaseAttributeSet::GetWisdomAttribute(), Wisdom);
	ATTRIBUTE(SPR, UBaseAttributeSet::GetSpiritAttribute(), Spirit);
	ATTRIBUTE(LCK, UBaseAttributeSet::GetLuckAttribute(), Luck);
	return Attributes;
}

TArray<FGameplayModifierInfo> UAttributeData::BuildVitalAttributes() const
{
	TArray<FGameplayModifierInfo> VitalAttributes;
	
	VITAL_ATTRIBUTE(HP, UBaseAttributeSet::GetMaxHealthAttribute(), MaxHealth);
	VITAL_ATTRIBUTE(MP, UBaseAttributeSet::GetMaxManaAttribute(), MaxMana);
	return VitalAttributes;
}

void UAttributeData::GiveToAbilitySystem(UAbilitySystemComponent* ASC, const float Level)
{
	BuildPrimaryAttributes();
	
	FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();
	EffectContext.AddSourceObject(ASC->GetAvatarActor());

	const FString EffectName = FString::Printf(TEXT("Attributes"));
	UGameplayEffect* Effect = NewObject<UGameplayEffect>(GetTransientPackage(), FName(EffectName));
	
	Effect->DurationPolicy = EGameplayEffectDurationType::Instant;
	Effect->Modifiers.Append(BuildPrimaryAttributes());
	
	const FActiveGameplayEffectHandle GameplayEffectHandle = ASC->ApplyGameplayEffectToSelf(Effect, Level, EffectContext);
	
	FGameplayEffectContextHandle VitalEffectContext = ASC->MakeEffectContext();
	VitalEffectContext.AddSourceObject(ASC->GetAvatarActor());

	const FString VitalEffectName = FString::Printf(TEXT("VitalAttributes"));
	UGameplayEffect* VitalEffect = NewObject<UGameplayEffect>(GetTransientPackage(), FName(VitalEffectName));
	
	VitalEffect->DurationPolicy = EGameplayEffectDurationType::Instant;
	VitalEffect->Modifiers.Append(BuildVitalAttributes());
	
	const FActiveGameplayEffectHandle VitalGameplayEffectHandle = ASC->ApplyGameplayEffectToSelf(VitalEffect, Level, VitalEffectContext);
}
