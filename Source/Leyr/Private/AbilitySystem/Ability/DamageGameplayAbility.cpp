// @ Retropsis 2024-2025.

#include "AbilitySystem/Ability/DamageGameplayAbility.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

void UDamageGameplayAbility::CauseDamage(AActor* TargetActor)
{
	FGameplayEffectSpecHandle DamageSpecHandle = MakeOutgoingGameplayEffectSpec(DamageEffectClass, GetAbilityLevel());
	// for (TTuple<FGameplayTag, FValueRange> Pair : DamageTypes)
	// {
	// 	const float ScaledMagnitude = Pair.Value.GetRandomFloatFromScalableRange(GetAbilityLevel());
	// 	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle, Pair.Key, ScaledMagnitude);
	// }
	// Single Type
	const float ScaledDamage =AbilityPower.GetRandomFloatFromScalableRange(GetAbilityLevel());
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle, DamageType, ScaledDamage);
	GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(*DamageSpecHandle.Data.Get(), UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor));
}

FAdditionalEffectParams UDamageGameplayAbility::MakeAdditionalEffectParamsFromClassDefaults(AActor* TargetActor) const
{
	FAdditionalEffectParams Params;
	Params.WorldContextObject = GetAvatarActorFromActorInfo();

	// TODO: Could Additional Effect Class be specialised ? (Using AdditionalEffectClass instead of DamageEffectClass)
	Params.AdditionalEffectClass = DamageEffectClass;
	Params.SourceAbilitySystemComponent = GetAbilitySystemComponentFromActorInfo();
	Params.TargetAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	Params.AbilityPower = AbilityPower.GetRandomFloatFromScalableRange(GetAbilityLevel());
	Params.AbilityLevel = GetAbilityLevel();
	Params.DamageType = DamageType;
	Params.StatusEffectChance = StatusEffectChance;
	Params.StatusEffectDamage = StatusEffectDamage;
	Params.StatusEffectDuration = StatusEffectDuration;
	Params.StatusEffectFrequency = StatusEffectFrequency;
	return Params;
}

FTaggedMontage UDamageGameplayAbility::GetRandomTaggedMontageFromArray(const TArray<FTaggedMontage>& TaggedMontages) const
{
	if (TaggedMontages.Num() > 0)
	{
		const int32 Selection = FMath::RandRange(0, TaggedMontages.Num() - 1);
		return TaggedMontages[Selection];
	}
	return FTaggedMontage();
}

// FVector2D UDamageGameplayAbility::GetDamageByDamageType(float InLevel, const FGameplayTag& DamageType)
// {
// 	checkf(DamageTypes.Contains(DamageType), TEXT("GameplayAbilit [%s] does not contain DamageType [%s]"), *GetNameSafe(this), *DamageType.ToString());
// 	return FVector2D(DamageTypes[DamageType].ValueMin.GetValueAtLevel(InLevel), DamageTypes[DamageType].ValueMax.GetValueAtLevel(InLevel));
// }
