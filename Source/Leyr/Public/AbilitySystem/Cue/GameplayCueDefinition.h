// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameplayCueDefinition.generated.h"

class UNiagaraSystem;
/**
 * 
 */
UCLASS(BlueprintType)
class LEYR_API UGameplayCueDefinition : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, meta=(ExposeOnSpawn))
	USoundBase* ImpactSound = nullptr;
	
	UPROPERTY(BlueprintReadOnly, meta=(ExposeOnSpawn))
	UNiagaraSystem* ImpactEffect = nullptr;
};
