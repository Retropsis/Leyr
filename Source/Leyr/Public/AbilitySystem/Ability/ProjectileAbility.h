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
	
	UFUNCTION(BlueprintCallable)
	void SetPitchOverride(bool bShouldOverride, const float Pitch);

	UPROPERTY(BlueprintReadWrite)
	FVector TargetLocation = FVector::ZeroVector;

	UPROPERTY(BlueprintReadWrite)
	bool bHasTarget = false;
	
protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UFUNCTION(BlueprintCallable, Category="Projectile", meta=(AdvancedDisplay=2))
	void SpawnProjectile(const FGameplayTag& SocketTag, const FVector& ProjectileTargetLocation);

	int32 NumProjectiles = 5;
	float ProjectileSpread = 90.f;
	int32 MaxNumProjectiles = 5;
	bool bLaunchHomingProjectiles = true;
	float HomingAccelerationMin = 1600.f;
	float HomingAccelerationMax = 3200.f;
	bool bOverridePitch = false;
	float PitchOverride = 0.f;
};
