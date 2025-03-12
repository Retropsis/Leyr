// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "DamageGameplayAbility.h"
#include "ProjectileAbility.generated.h"

class AProjectile;

/**
 * 
 */
UCLASS()
class LEYR_API UProjectileAbility : public UDamageGameplayAbility
{
	GENERATED_BODY()

public:
	virtual void InitAbility() override;
	
protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UFUNCTION(BlueprintCallable, Category="Projectile")
	void SpawnProjectile(const FVector& ProjectileTargetLocation, const FGameplayTag& SocketTag, bool bIgnoreStatic = false, bool bOverridePitch = false, float PitchOverride = 0.f);

	UPROPERTY(EditDefaultsOnly)
	int32 NumProjectiles = 5;

	/*
	 * Projectile Ability Data
	*/	
	TSubclassOf<AProjectile> ProjectileClass;
	bool bIgnoreStatic = false;
	bool bOverridePitch = false;
	float PitchOverride = 0.f;
};
