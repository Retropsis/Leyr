// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/BaseGameplayAbility.h"
#include "AbilitySystem/Data/AbilityDescriptionData.h"
#include "MasteryAbility.generated.h"

/**
 * 
 */
UCLASS()
class LEYR_API UMasteryAbility : public UBaseGameplayAbility
{
	GENERATED_BODY()

public:
	virtual void PrepareToEndAbility() override;
	
	UFUNCTION(BlueprintCallable)
	void ApplyMasteryEffect();
	
	virtual FString GetDescription(int32 Level) override;
	virtual FString GetNextLevelDescription(int32 Level) override;

	/*
	 * Damage Ability Data
	 */
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> MasteryEffectClass;
	
	UPROPERTY(EditDefaultsOnly)
	FAbilityDescription Info;

private:
	FActiveGameplayEffectHandle ActiveGameplayEffectHandle;
};
