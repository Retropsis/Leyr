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

// FAttributeBasedFloat AttributeBasedFloat##Name; \
// AttributeBasedFloat##Name.Coefficient = AttributeData.Coefficient; \
// AttributeBasedFloat##Name.PreMultiplyAdditiveValue = AttributeData.PreMultiplyAdditiveValue; \
// AttributeBasedFloat##Name.PostMultiplyAdditiveValue = AttributeData.PostMultiplyAdditiveValue; \
// AttributeBasedFloat##Name.BackingAttribute = FGameplayEffectAttributeCaptureDefinition(AttributeData.BackingAttribute, EGameplayEffectAttributeCaptureSource::Source, false); \
// ModifierInfo##Name.ModifierMagnitude = AttributeBasedFloat##Name; \

void UAttributeData::BuildAttributes()
{
	Attributes.Empty();
	VitalAttributes.Empty();
	ATTRIBUTE(STR, UBaseAttributeSet::GetStrengthAttribute(), Strength);
	ATTRIBUTE(DEX, UBaseAttributeSet::GetDexterityAttribute(), Dexterity);
	ATTRIBUTE(VIT, UBaseAttributeSet::GetVitalityAttribute(), Vitality);
	ATTRIBUTE(INT, UBaseAttributeSet::GetIntelligenceAttribute(), Intelligence);
	ATTRIBUTE(WIS, UBaseAttributeSet::GetWisdomAttribute(), Wisdom);
	ATTRIBUTE(SPR, UBaseAttributeSet::GetSpiritAttribute(), Spirit);
	ATTRIBUTE(LCK, UBaseAttributeSet::GetLuckAttribute(), Luck);
	VITAL_ATTRIBUTE(HP, UBaseAttributeSet::GetMaxHealthAttribute(), MaxHealth);
	VITAL_ATTRIBUTE(MP, UBaseAttributeSet::GetMaxManaAttribute(), MaxMana);
	
	// FGameplayModifierInfo ModifierInfoTest; \
	// ModifierInfoTest.Attribute = UBaseAttributeSet::GetMaxHealthAttribute(); 
	// ModifierInfoTest.ModifierOp = EGameplayModOp::Override;
	// FCustomCalculationBasedFloat CustomCalculationBasedFloat;
	// CustomCalculationBasedFloat.Coefficient = MaxHealth.Coefficient;
	// CustomCalculationBasedFloat.PreMultiplyAdditiveValue = MaxHealth.PreMultiplyAdditiveValue; 
	// CustomCalculationBasedFloat.PostMultiplyAdditiveValue = MaxHealth.PostMultiplyAdditiveValue; 
	// CustomCalculationBasedFloat.CalculationClassMagnitude = MaxHealth.CalculationClassMagnitude;
	// ModifierInfoTest.ModifierMagnitude = CustomCalculationBasedFloat; 
	// VitalAttributes.Add(ModifierInfoTest); 
}

void UAttributeData::GiveToAbilitySystem(UAbilitySystemComponent* ASC, const float Level)
{
	BuildAttributes();
	
	FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();
	EffectContext.AddSourceObject(ASC->GetAvatarActor());

	const FString EffectName = FString::Printf(TEXT("Attributes"));
	UGameplayEffect* Effect = NewObject<UGameplayEffect>(GetTransientPackage(), FName(EffectName));
	
	Effect->DurationPolicy = EGameplayEffectDurationType::Instant;
	Effect->StackingType = EGameplayEffectStackingType::AggregateBySource;
	Effect->StackLimitCount = 1;
	Effect->Modifiers.Append(Attributes);
	
	// const UGameplayEffect* GameplayEffect = EffectToGrant.GameplayEffect->GetDefaultObject<UGameplayEffect>();
	const FActiveGameplayEffectHandle GameplayEffectHandle = ASC->ApplyGameplayEffectToSelf(Effect, Level, EffectContext);
	
	const FString VitalAttributesEffectName = FString::Printf(TEXT("VitalAttributes"));
	UGameplayEffect* VitalAttributesEffect = NewObject<UGameplayEffect>(GetTransientPackage(), FName(VitalAttributesEffectName));
	
	VitalAttributesEffect->DurationPolicy = EGameplayEffectDurationType::Instant;
	VitalAttributesEffect->StackingType = EGameplayEffectStackingType::AggregateBySource;
	VitalAttributesEffect->StackLimitCount = 1;
	VitalAttributesEffect->Modifiers.Append(VitalAttributes);
	
	// const UGameplayEffect* GameplayEffect = EffectToGrant.GameplayEffect->GetDefaultObject<UGameplayEffect>();
	FGameplayEffectContextHandle VItalAttributesEffectContext = ASC->MakeEffectContext();
	VItalAttributesEffectContext.AddSourceObject(ASC->GetAvatarActor());
	const FActiveGameplayEffectHandle VitalAttributesEffectHandle = ASC->ApplyGameplayEffectToSelf(VitalAttributesEffect, Level, VItalAttributesEffectContext);
}
