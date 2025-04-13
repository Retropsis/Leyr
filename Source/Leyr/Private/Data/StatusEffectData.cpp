// @ Retropsis 2024-2025.

#include "Data/StatusEffectData.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Game/BaseGameplayTags.h"

FGameplayEffectContextHandle UStatusEffectData::ApplyStatusEffect(UAbilitySystemComponent* SourceASC, UAbilitySystemComponent* TargetASC)
{
	const FBaseGameplayTags& GameplayTags = FBaseGameplayTags::Get();
	// const AActor* SourceAvatarActor = StatusEffectParams.SourceAbilitySystemComponent->GetAvatarActor();

	FGameplayEffectContextHandle EffectContextHandle = SourceASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(SourceASC);
	
	const FGameplayEffectSpecHandle SpecHandle = TargetASC->MakeOutgoingSpec(StatusEffectClass, StatusEffectLevel, EffectContextHandle);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, StatusEffectType, StatusEffectPower.GetValueAtLevel(StatusEffectLevel));
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, AdditionalDamageType, StatusEffectPower.GetValueAtLevel(StatusEffectLevel));
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.StatusEffect_Chance, StatusEffectChance);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.StatusEffect_Damage, StatusEffectDamage);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.StatusEffect_Duration, StatusEffectDuration);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.StatusEffect_Frequency, StatusEffectFrequency);

	TargetASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data);
	return EffectContextHandle;
}
