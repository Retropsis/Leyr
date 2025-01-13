// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "MeleeGameplayAbility.h"
#include "MultiHitMeleeAbility.generated.h"

/**
 * 
 */
UCLASS()
class LEYR_API UMultiHitMeleeAbility : public UMeleeGameplayAbility
{
	GENERATED_BODY()

public:	
	UFUNCTION(BlueprintCallable)
	void SelectMontageTagCombo();
	
	UFUNCTION(BlueprintCallable)
	void OnComboWindowBegin();
	
	UFUNCTION(BlueprintCallable)
	void OnComboWindowEnd();
	
	UPROPERTY(BlueprintReadWrite)
	bool bShouldEndAbility = true;
};
