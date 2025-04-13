// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "GameplayTagContainer.h"
#include "ScalableFloat.h"
#include "Engine/DataAsset.h"
#include "StatusEffectData.generated.h"

class UGameplayEffect;
/**
 * 
 */
UCLASS()
class LEYR_API UStatusEffectData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	FGameplayEffectContextHandle ApplyStatusEffect(UAbilitySystemComponent* SourceASC, UAbilitySystemComponent* TargetASC);
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> StatusEffectClass = nullptr;
	
	UPROPERTY(EditDefaultsOnly, meta=(Categories="StatusEffect"))
	FGameplayTag StatusEffectType = FGameplayTag();
	
	UPROPERTY(EditDefaultsOnly, meta=(Categories="Damage"))
	FGameplayTag AdditionalDamageType = FGameplayTag();
	
	UPROPERTY(EditDefaultsOnly)
	float StatusEffectLevel = 1.f;
	
	UPROPERTY(EditDefaultsOnly)
	FScalableFloat StatusEffectPower = 1.f;
	
	UPROPERTY(EditDefaultsOnly, meta=(ClampMin=0.f, ClampMax=100.f))
	float StatusEffectChance = 20.f;

	UPROPERTY(EditDefaultsOnly)
	float StatusEffectDamage = 5.f;

	UPROPERTY(EditDefaultsOnly)
	float StatusEffectFrequency = 1.f;

	UPROPERTY(EditDefaultsOnly)
	float StatusEffectDuration = 5.f;

	/*
	 * Impulse on defeat
	 */
	UPROPERTY(EditDefaultsOnly)
	float DeathImpulseMagnitude = 250.f;

	/*
	 * Airborne
	*/
	UPROPERTY(EditDefaultsOnly, meta=(ClampMin=0.f, ClampMax=100.f))
	float AirborneChance = 0.f;
	
	UPROPERTY(EditDefaultsOnly)
	float AirborneForceMagnitude = 250.f;
};
