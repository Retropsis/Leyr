// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "World/Level/Spawner/EncounterSpawnVolume.h"
#include "EncounterSpawnData.generated.h"

enum class ESpawnerType : uint8;
class AAICharacter;
class UBehaviourData;
class UEncounterData;

USTRUCT(BlueprintType)
struct FEncounterSpawn
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AAICharacter> EncounterClass = nullptr;
	
	UPROPERTY(EditDefaultsOnly)
	int32 Level = 1;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UEncounterData> EncounterData = nullptr;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UBehaviourData> OverrideBehaviourData = nullptr;

	UPROPERTY(EditDefaultsOnly)
	float RespawnTime = 8.f;
};

/**
 * 
 */
UCLASS(BlueprintType)
class LEYR_API UEncounterSpawnData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	bool bRandomizeLocation = false;
	
	UPROPERTY(EditDefaultsOnly)
	ESpawnerType SpawnerType = ESpawnerType::Once; 
	
	UPROPERTY(EditDefaultsOnly)
	TArray<FEncounterSpawn> EncounterSpawns;
};
