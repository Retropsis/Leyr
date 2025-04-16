// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LevelAreaData.generated.h"

class UGameplayEffect;
class UEncounterSpawnData;
/**
 * 
 */
UCLASS(BlueprintType)
class LEYR_API ULevelAreaData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	TArray<TObjectPtr<UEncounterSpawnData>> SpawningVolumes;

	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<UGameplayEffect>> GrantedGameplayEffects;
};
