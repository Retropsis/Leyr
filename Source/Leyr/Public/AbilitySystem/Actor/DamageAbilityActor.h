// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Actor/AbilityActor.h"
#include "DamageAbilityActor.generated.h"

/**
 * 
 */
UCLASS()
class LEYR_API ADamageAbilityActor : public AAbilityActor
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void CauseDamage(AActor* TargetActor);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="AbilityActor")
	FScalableFloat AbilityPower;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="AbilityActor")
	TSubclassOf<UGameplayEffect> DamageEffectClass;
	
	UPROPERTY(EditDefaultsOnly, Category="AbilityActor", meta=(Categories="Damage"))
	FGameplayTag DamageType;
	
	UPROPERTY(EditDefaultsOnly, Category="AbilityActor", meta=(Categories="Damage"))
	bool bShouldApplyInvincibility = false;	
};
