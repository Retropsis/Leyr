// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "AbilitySystem/Data/AttackSequenceInfo.h"
#include "Engine/DataAsset.h"
#include "AbilityData.generated.h"

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
	
	UPROPERTY(EditDefaultsOnly, Category="Ability")
	FGameplayTag DamageType = FGameplayTag();
	
	UPROPERTY(EditDefaultsOnly, Category="Ability")
	ESequenceType SequenceType = ESequenceType::Default;

	UPROPERTY(EditDefaultsOnly, Category="Ability")
	float AbilityPower = 0.f;
	
	UPROPERTY(EditDefaultsOnly, Category="Ability")
	float AbilityPoise = 0.f;

	/*
	 * Projectile
	*/
	UPROPERTY(EditDefaultsOnly, Category="Ability|Projectile")
	TSubclassOf<AProjectile> ProjectileClass = nullptr;
	
	UPROPERTY(EditDefaultsOnly, Category="Ability|Projectile")
	bool bOverridePitch = false;
	
	UPROPERTY(EditDefaultsOnly, Category="Ability|Projectile")
	float PitchOverride = 0.f;
	
	UPROPERTY(EditDefaultsOnly, Category="Ability|Projectile")
	bool bIgnoreStatic = false;
};
