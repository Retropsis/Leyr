// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/DamageGameplayAbility.h"
#include "MeleeGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class LEYR_API UMeleeGameplayAbility : public UDamageGameplayAbility
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	TArray<FHitResult> BoxTrace();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(Categories="Montage"))
	FGameplayTag MontageTag = FGameplayTag();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(Categories="Montage"))
	FGameplayTag MainMontageTag = FGameplayTag();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(Categories="Montage"))
	FGameplayTag AlternativeMontageTag = FGameplayTag();

protected:
	UPROPERTY(EditDefaultsOnly)
	bool bDebugBoxTrace = false;

private:
	FBoxTraceData BoxTraceData = FBoxTraceData();
};
