// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/BaseGameplayAbility.h"
#include "ParryAbility.generated.h"

/**
 * 
 */
UCLASS()
class LEYR_API UParryAbility : public UBaseGameplayAbility
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable)
	void TryParry();
};
