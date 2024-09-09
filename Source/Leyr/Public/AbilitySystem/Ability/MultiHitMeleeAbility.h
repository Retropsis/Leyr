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
	UPROPERTY(BlueprintReadWrite)
	int32 ComboSequenceIndex = 0;
	
	UPROPERTY(BlueprintReadWrite)
	bool bComboActivated = false;
};
