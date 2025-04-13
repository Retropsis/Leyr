// @ Retropsis 2024-2025.

#include "Data/AbilityData.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/LeyrAbilitySystemLibrary.h"
#include "Game/BaseGameplayTags.h"

FGameplayEffectContextHandle UAbilityData::ApplyAdditionalEffect(UAbilitySystemComponent* SourceASC, UAbilitySystemComponent* TargetASC)
{
	const FBaseGameplayTags& GameplayTags = FBaseGameplayTags::Get();
	// const AActor* SourceAvatarActor = SourceAbilitySystemComponent->GetAvatarActor();

	FGameplayEffectContextHandle EffectContextHandle = SourceASC->MakeEffectContext();
	// EffectContextHandle.AddSourceObject(SourceASC);
	// ULeyrAbilitySystemLibrary::SetDeathImpulse(EffectContextHandle, DeathImpulse);
	// ULeyrAbilitySystemLibrary::SetAirborneForce(EffectContextHandle, AirborneForce);
	//
	// ULeyrAbilitySystemLibrary::SetIsRadialDamage(EffectContextHandle, bIsRadialDamage);
	// ULeyrAbilitySystemLibrary::SetRadialDamageInnerRadius(EffectContextHandle, RadialDamageInnerRadius);
	// ULeyrAbilitySystemLibrary::SetRadialDamageOuterRadius(EffectContextHandle, RadialDamageOuterRadius);
	// ULeyrAbilitySystemLibrary::SetRadialDamageOrigin(EffectContextHandle, RadialDamageOrigin);
	//
	// const FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(AdditionalEffectClass, AbilityLevel, EffectContextHandle);
	// UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, DamageType, AbilityPower);
	//
	// TargetASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data);
	return EffectContextHandle;
}
