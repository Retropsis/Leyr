// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Leyr/Leyr.h"
#include "BaseGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class LEYR_API UBaseGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, Category="Input", meta=(Categories="InputTag"))
	FGameplayTag StartupInputTag;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Ability|Damage")
	FValueRange AbilityPower;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector CursorHitLocation;
};
