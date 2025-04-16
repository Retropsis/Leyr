// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "EncounterSpawnData.generated.h"

class AAICharacter;
class UBehaviourData;
class UEncounterData;

UENUM(BlueprintType)
enum class ESpawnerType : uint8
{
	None,
	Once,
	Infinite,
};

UENUM(BlueprintType)
enum class ESpawnLocationType : uint8
{
	Point,
	AroundPoint,
	Random,
	RandomCloseToPlayer,
};

USTRUCT(BlueprintType)
struct FEncounterSpawn
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AAICharacter> EncounterClass = nullptr;
	
	UPROPERTY(EditDefaultsOnly, meta=(ClampMin=1))
	int32 Level = 1;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UEncounterData> EncounterData = nullptr;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UBehaviourData> OverrideBehaviourData = nullptr;

	UPROPERTY(EditDefaultsOnly)
	float RespawnTime = 8.f;
	
	UPROPERTY(EditDefaultsOnly, meta=(ClampMin=1))
	int32 Count = 1;
	
	UPROPERTY(EditDefaultsOnly)
	ESpawnLocationType SpawnLocationType = ESpawnLocationType::Point;
	
	UPROPERTY(EditDefaultsOnly)
	ESpawnerType SpawnerType = ESpawnerType::Once;

	UPROPERTY(EditDefaultsOnly)
	float PreferredSpawningRange = 300.f;
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
	TArray<FEncounterSpawn> EncounterSpawns;
};
