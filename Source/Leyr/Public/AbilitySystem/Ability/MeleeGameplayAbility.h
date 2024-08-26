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
	FHitResult BoxTrace();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Ability|BoxTrace")
	FVector BoxTraceExtent = FVector(40.f, 20.f, 30.f);

protected:
	UPROPERTY(BlueprintReadWrite)
	FVector BoxTraceStart = FVector::ZeroVector;
	
	UPROPERTY(BlueprintReadWrite)
	FVector BoxTraceEnd = FVector::ZeroVector;
};
