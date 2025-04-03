// @ Retropsis 2024-2025.

#include "AbilitySystem/Actor/DamageAbilityActor.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/LeyrAbilitySystemLibrary.h"
#include "Game/BaseGameplayTags.h"

void ADamageAbilityActor::CauseDamage(AActor* TargetActor)
{
	UAbilitySystemComponent* SourceASC = GetAbilitySystemComponent();
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);

	if(TargetASC && TargetASC->HasMatchingGameplayTag(FBaseGameplayTags::Get().Invincibility)) return;
	
	const FGameplayEffectSpecHandle DamageSpecHandle = SourceASC->MakeOutgoingSpec(DamageEffectClass, Level, SourceASC->MakeEffectContext());	
	const float ScaledDamage = AbilityPower.GetValueAtLevel(Level);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle, DamageType, ScaledDamage);
	SourceASC->ApplyGameplayEffectSpecToTarget(*DamageSpecHandle.Data.Get(), TargetASC);

	if (bShouldApplyInvincibility) ULeyrAbilitySystemLibrary::ApplyInvincibilityToTarget(TargetASC, 1.25f);
}
