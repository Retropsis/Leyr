// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "ScalableFloat.h"
#include "AbilitySystem/Data/AttackSequenceInfo.h"
#include "Engine/DataAsset.h"
#include "AbilityData.generated.h"

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
	UPROPERTY(EditDefaultsOnly, Category="Ability")
	TSubclassOf<UGameplayEffect> MainEffectClass = nullptr;
	
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
	bool bShouldApplyExecute = false;
	
	UPROPERTY(EditDefaultsOnly, Category="Ability")
	bool bShouldExecute = false;

	UPROPERTY(EditDefaultsOnly, Category="Ability|Range")
	TObjectPtr<UProjectileData> ProjectileData = nullptr;
	
	UPROPERTY(EditDefaultsOnly, Category="Ability|Range", meta=(EditCondition="ProjectileData != nullptr", EditConditionHides))
	float ProjectileSpread = 90.f;
 
	UPROPERTY(EditDefaultsOnly, Category="Ability|Range", meta=(EditCondition="ProjectileData != nullptr", EditConditionHides))
	int32 MaxNumProjectiles = 5;

	/*
	 * Homing
	 */
	UPROPERTY(EditDefaultsOnly, Category="Ability|Range", meta=(EditCondition="ProjectileData != nullptr", EditConditionHides))
	bool bLaunchHomingProjectiles = true;
	
	UPROPERTY(EditDefaultsOnly, Category="Ability|Range", meta=(EditCondition="ProjectileData != nullptr", EditConditionHides))
	float HomingAccelerationMin = 1600.f;
 
	UPROPERTY(EditDefaultsOnly, Category="Ability|Range", meta=(EditCondition="ProjectileData != nullptr", EditConditionHides))
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
	UPROPERTY(EditDefaultsOnly, Category="Ability|Range", meta=(EditCondition="ProjectileData != nullptr", EditConditionHides))
	bool bOverridePitch = false;
	
	UPROPERTY(EditDefaultsOnly, Category="Ability|Range", meta=(EditCondition="ProjectileData != nullptr", EditConditionHides))
	float PitchOverride = 0.f;
};
