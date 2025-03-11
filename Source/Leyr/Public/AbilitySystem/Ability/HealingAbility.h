// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/BaseGameplayAbility.h"
#include "HealingAbility.generated.h"

/**
 * 
 */
UCLASS()
class LEYR_API UHealingAbility : public UBaseGameplayAbility
{
	GENERATED_BODY()
	
	UFUNCTION(BlueprintCallable)
	void CauseHealing(UAbilitySystemComponent* TargetASC);

protected:
	virtual void InitAbility() override;

	/*
	 * Healing Ability Data
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Ability|Healing")
	TSubclassOf<UGameplayEffect> HealingEffectClass;

	UPROPERTY(EditDefaultsOnly, Category="Ability|Healing", meta=(Categories="Healing"))
	FGameplayTag HealingType;
};
