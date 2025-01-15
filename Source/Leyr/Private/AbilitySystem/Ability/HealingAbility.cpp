// @ Retropsis 2024-2025.

#include "AbilitySystem/Ability/HealingAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Game/BaseGameplayTags.h"

void UHealingAbility::CauseHealing(UAbilitySystemComponent* TargetASC)
{
	FGameplayEffectSpecHandle DamageSpecHandle = MakeOutgoingGameplayEffectSpec(HealingEffectClass, GetAbilityLevel());
	const float ScaledDamage =AbilityPower.GetRandomFloatFromScalableRange(GetAbilityLevel());
	
	if (TargetASC->HasMatchingGameplayTag(FBaseGameplayTags::Get().Indicator_Execute))
	{
		HealingType = FBaseGameplayTags::Get().Damage_Execute;
	}
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle, HealingType, ScaledDamage);
	// TODO: Execute here would be good against undead
	// if(bShouldApplyExecute) MakeAndApplyExecuteEffectToTarget(FBaseGameplayTags::Get().Indicator_Execute, TargetASC);
	
	GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(*DamageSpecHandle.Data.Get(), TargetASC);
}
