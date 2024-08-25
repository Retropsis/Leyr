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
	
	virtual FString GetDescription(int32 Level);
	virtual FString GetNextLevelDescription(int32 Level);
	static FString GetLockedDescription(int32 Level);

protected:
	float GetManaCost(float InLevel = 1.f) const;
	float GetCooldown(float InLevel = 1.f) const;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Ability|Damage")
	FValueRange AbilityPower;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector CursorHitLocation;
};
