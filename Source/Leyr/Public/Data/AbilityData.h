// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "GameplayTagContainer.h"
#include "ScalableFloat.h"
#include "AbilitySystem/Data/AttackSequenceInfo.h"
#include "Engine/DataAsset.h"
#include "AbilityData.generated.h"

class UStatusEffectData;
class UProjectileData;
class UGameplayEffect;
class AProjectile;
/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class LEYR_API UAbilityData : public UDataAsset
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	FGameplayEffectContextHandle ApplyAdditionalEffect(UAbilitySystemComponent* SourceASC, UAbilitySystemComponent* TargetASC);
	
	UPROPERTY(EditDefaultsOnly, Category="Ability")
	TSubclassOf<UGameplayEffect> DamageEffectClass = nullptr;
	
	UPROPERTY(EditDefaultsOnly, Category="Ability", meta=(Categories=Damage))
	FGameplayTag DamageType = FGameplayTag();
	
	UPROPERTY(EditDefaultsOnly, Category="Ability", meta=(Categories=Healing))
	FGameplayTag HealingType = FGameplayTag();
	
	UPROPERTY(EditDefaultsOnly, Category="Ability")
	ESequenceType SequenceType = ESequenceType::Default;

	UPROPERTY(EditDefaultsOnly, Category="Ability")
	FScalableFloat AbilityPower;
	
	UPROPERTY(EditDefaultsOnly, Category="Ability")
	float AbilityPoise = 0.f;
	
	UPROPERTY(EditDefaultsOnly, Category="Ability")
	float CharacterLevelRatio = 1.f;
	
	UPROPERTY(EditDefaultsOnly, Category="Ability")
	float AbilityLevelRatio = 1.f;
	
	UPROPERTY(EditDefaultsOnly, Category="Ability")
	bool bShouldApplyExecute = false;
	
	UPROPERTY(EditDefaultsOnly, Category="Ability")
	bool bShouldExecute = false;

	UPROPERTY(EditDefaultsOnly, Category="Ability|StatusEffect", meta=(InlineEditConditionToggle))
	bool bApplyStatusEffect = false;
	
	UPROPERTY(EditDefaultsOnly, Category="Ability|StatusEffect", meta=(EditCondition="bApplyStatusEffect"))
	TObjectPtr<UStatusEffectData> StatusEffectData = nullptr;
	
	UPROPERTY(EditDefaultsOnly, Category="Ability|Range")
	TSubclassOf<AProjectile> ProjectileClass = nullptr;
	
	UPROPERTY(EditDefaultsOnly, Category="Ability|Range", meta=(EditCondition="ProjectileClass != nullptr", EditConditionHides))
	float ProjectileSpread = 90.f;
 
	UPROPERTY(EditDefaultsOnly, Category="Ability|Range", meta=(EditCondition="ProjectileClass != nullptr", EditConditionHides))
	int32 MaxNumProjectiles = 5;

	/*
	 * Homing
	 */
	UPROPERTY(EditDefaultsOnly, Category="Ability|Range", meta=(EditCondition="ProjectileClass != nullptr", EditConditionHides))
	bool bLaunchHomingProjectiles = true;
	
	UPROPERTY(EditDefaultsOnly, Category="Ability|Range", meta=(EditCondition="ProjectileClass != nullptr", EditConditionHides))
	float HomingAccelerationMin = 1600.f;
 
	UPROPERTY(EditDefaultsOnly, Category="Ability|Range", meta=(EditCondition="ProjectileClass != nullptr", EditConditionHides))
	float HomingAccelerationMax = 3200.f;

	/*
	 * Radial
	 */
	UPROPERTY(EditDefaultsOnly, Category="Ability|Radial")
	bool bIsRadialDamage = false;
 
	UPROPERTY(EditDefaultsOnly, Category="Ability|Radial", meta=(EditCondition="bIsRadialDamage", EditConditionHides))
	float RadialDamageInnerRadius = 0.f;
 
	UPROPERTY(EditDefaultsOnly, Category="Ability|Radial", meta=(EditCondition="bIsRadialDamage", EditConditionHides))
	float RadialDamageOuterRadius = 0.f;

	/*
	 * Pitch
	 */
	UPROPERTY(EditDefaultsOnly, Category="Ability|Range", meta=(EditCondition="ProjectileClass != nullptr", EditConditionHides))
	bool bOverridePitch = false;
	
	UPROPERTY(EditDefaultsOnly, Category="Ability|Range", meta=(EditCondition="ProjectileClass != nullptr", EditConditionHides))
	float PitchOverride = 0.f;

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
