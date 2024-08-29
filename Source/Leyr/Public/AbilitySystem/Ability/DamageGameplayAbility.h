// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "AbilityTypes.h"
#include "AbilitySystem/Ability/BaseGameplayAbility.h"
#include "Interaction/CombatInterface.h"
#include "DamageGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class LEYR_API UDamageGameplayAbility : public UBaseGameplayAbility
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void CauseDamage(AActor* TargetActor);

	UFUNCTION(BlueprintPure)
	FAdditionalEffectParams MakeAdditionalEffectParamsFromClassDefaults(AActor* TargetActor = nullptr) const;
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Ability|Damage")
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	// UPROPERTY(EditDefaultsOnly, Category="Ability|Damage", meta=(Categories="Damage"))
	// TMap<FGameplayTag, FValueRange> DamageTypes;

	UPROPERTY(EditDefaultsOnly, Category="Ability|Damage", meta=(Categories="Damage"))
	FGameplayTag DamageType;

	UPROPERTY(EditDefaultsOnly, Category="Ability|Status Effect")
	float StatusEffectChance = 20.f;

	UPROPERTY(EditDefaultsOnly, Category="Ability|Status Effect")
	float StatusEffectDamage = 5.f;

	UPROPERTY(EditDefaultsOnly, Category="Ability|Status Effect")
	float StatusEffectFrequency = 1.f;

	UPROPERTY(EditDefaultsOnly, Category="Ability|Status Effect")
	float StatusEffectDuration = 5.f;
	
	UPROPERTY(EditDefaultsOnly, Category="Ability|Status Effect")
	float DeathImpulseMagnitude = 250.f;

	UPROPERTY(EditDefaultsOnly, Category="Ability|Status Effect")
	float AirborneForceMagnitude = 250.f;

	UPROPERTY(EditDefaultsOnly, Category="Ability|Status Effect")
	float AirborneChance = 0.f;

	UFUNCTION(BlueprintPure)
	FTaggedMontage GetRandomTaggedMontageFromArray(const TArray<FTaggedMontage>& TaggedMontages) const;
	
	// FVector2D GetDamageByDamageType(float InLevel, const FGameplayTag& DamageType);
};