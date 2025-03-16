// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
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
	
	UPROPERTY(EditDefaultsOnly, Category="Ability|Projectile")
	bool bOverridePitch = false;
	
	UPROPERTY(EditDefaultsOnly, Category="Ability|Projectile")
	float PitchOverride = 0.f;

	UPROPERTY(EditDefaultsOnly, Category="Ability")
	TObjectPtr<UProjectileData> ProjectileData = nullptr;
};
