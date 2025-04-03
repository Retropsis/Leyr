// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "ProjectileData.generated.h"

class UStatusEffectData;

UENUM(BlueprintType)
enum class EResponseToStatic : uint8
{
	Ignore,
	Destroy,
	Stop,
};

class UNiagaraSystem;
class AProjectile;
/**
 * 
 */
UCLASS()
class LEYR_API UProjectileData : public UDataAsset
{
	GENERATED_BODY()
	
public:	
	UPROPERTY(EditDefaultsOnly, Category="Projectile", meta=(Categories="Damage"))
	FGameplayTag DamageType = FGameplayTag();

	UPROPERTY(EditDefaultsOnly, Category="Projectile")
	TObjectPtr<UStatusEffectData> StatusEffectData = nullptr;
	
	UPROPERTY(EditDefaultsOnly, Category="Projectile")
	EResponseToStatic ResponseToStatic = EResponseToStatic::Destroy;
	
	UPROPERTY(EditDefaultsOnly, Category="Projectile")
	float ProjectileGravityScale = 0.f;
	
	UPROPERTY(EditDefaultsOnly, Category="Projectile")
	float InitialSpeed = 1200.f;
	
	UPROPERTY(EditDefaultsOnly, Category="Projectile")
	float MaxSpeed = 1200.f;

	UPROPERTY(EditDefaultsOnly, Category="Projectile")
	TObjectPtr<UNiagaraSystem> ImpactEffect = nullptr;
	
	UPROPERTY(EditDefaultsOnly, Category="Projectile")
	TObjectPtr<USoundBase> ImpactSound = nullptr;
	
	UPROPERTY(EditDefaultsOnly, Category="Projectile")
	TObjectPtr<USoundBase> LoopingSound = nullptr;
	
};
