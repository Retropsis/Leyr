// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "EventEffectData.generated.h"

class UNiagaraSystem;

USTRUCT(BlueprintType, Blueprintable)
struct FEventEffectData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UNiagaraSystem> VisualEffect;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USoundBase> SoundEffect;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SpawningChance = 0.5f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Direction = FVector::ZeroVector;
};
