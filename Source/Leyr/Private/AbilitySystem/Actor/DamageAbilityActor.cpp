// @ Retropsis 2024-2025.

#include "AbilitySystem/Actor/DamageAbilityActor.h"
#include "AbilitySystemBlueprintLibrary.h"

void ADamageAbilityActor::CauseDamage(AActor* TargetActor)
{
	UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
	const FGameplayEffectSpecHandle DamageSpecHandle = ASC->MakeOutgoingSpec(DamageEffectClass, Level, ASC->MakeEffectContext());
	const float ScaledDamage =AbilityPower.GetRandomFloatFromScalableRange(Level);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle, DamageType, ScaledDamage);
	ASC->ApplyGameplayEffectSpecToTarget(*DamageSpecHandle.Data.Get(), UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor));
}
