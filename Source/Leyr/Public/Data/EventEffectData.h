// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "EventEffectData.generated.h"

UENUM(BlueprintType, Blueprintable)
enum class EDesiredLocation  : uint8
{
	None,
	ActorLocation,
	ActorFeetLocation,
	CombatSocket,
	OffsetLocation,
};

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
	float Cooldown = 2.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EDesiredLocation DesiredLocation = EDesiredLocation::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector OffsetLocation = FVector::ZeroVector;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Direction = FVector::ZeroVector;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(Categories="CombatSocket"))
	FGameplayTag CombatSocketTag = FGameplayTag();
};
