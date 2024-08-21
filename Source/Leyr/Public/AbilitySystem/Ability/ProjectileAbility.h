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
	
protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UFUNCTION(BlueprintCallable, Category="Projectile")
	void SpawnProjectile(const FVector& ProjectileTargetLocation, const FGameplayTag& SocketTag, bool bOverridePitch = false, float PitchOverride = 0.f);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Ability")
	TSubclassOf<AProjectile> ProjectileClass;
};
