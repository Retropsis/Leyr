// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EffectSpawner.generated.h"

UENUM(BlueprintType)
enum class EEffectSpawnRule : uint8
{
	Random,
	All,
};

UENUM(BlueprintType)
enum class EEffectSpawnFrequency : uint8
{
	Always,
	Roll,
};

class UNiagaraSystem;

UCLASS()
class LEYR_API AEffectSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	AEffectSpawner();

	UFUNCTION(BlueprintCallable)
	void SpawnEffects(UPrimitiveComponent* OverlappedComponent, const FHitResult& HitResult);
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Effects")
	EEffectSpawnRule SpawnRule = EEffectSpawnRule::Random;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Effects")
	EEffectSpawnFrequency SpawnFrequency = EEffectSpawnFrequency::Roll;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Effects")
	float SpawnChance = 33.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Effects")
	TArray<TObjectPtr<USoundBase>> SoundEffects;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Effects")
	TArray<TObjectPtr<UNiagaraSystem>> NiagaraEffects;
};
