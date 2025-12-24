// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "EncounterSpawnData.generated.h"

class APointCollection;
class AAICharacter;
class UBehaviourData;
class UEncounterData;

UENUM(BlueprintType)
enum class ESpawnerType : uint8
{
	None,
	OnlyOnce,			/* Spawn only once per begin overlap */
	// OnlyOnce,		/* Unique spawn, destroy volume */
	Infinite,				/* Infinite respawn */
};

UENUM(BlueprintType)
enum class ESpawnLocationType : uint8
{
	SelectedPoint,					/* Chosen by placing a valid SpawnPoint */
	SelectedPointAroundPlayer,						/* Chosen randomly around a valid SpawnPoint */
	PointCollection,				/* Chosen by using a collection of points, by index order */
	PointCollectionRandom, /* Chosen by using a collection of points, randomly */
	Random,							/* Randomly chosen within the spawning bounds */
	RandomCloseToPlayer,	/* Randomly chosen within the spawning bounds, preferably close to player */
	OutOfBounds,					/* Chosen by placing a valid SpawnPoint */
	OutOfBoundsFixed,			/*  */
};

/**
 * 
 */
UCLASS(BlueprintType)
class LEYR_API UEncounterSpawnData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, meta = (Categories="EncounterSpawn"))
	FGameplayTag EncounterSpawnTag = FGameplayTag::EmptyTag;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AAICharacter> EncounterClass = nullptr;
	
	UPROPERTY(EditDefaultsOnly, meta=(ClampMin=1))
	int32 Level = 1;
	
	UPROPERTY(EditDefaultsOnly, meta=(ClampMin=1))
	int32 Count = 1;

	UPROPERTY(EditDefaultsOnly)
	float RespawnTime = 8.f;

	UPROPERTY(EditDefaultsOnly)
	float PreferredSpawningRange = 300.f;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UEncounterData> EncounterData = nullptr;
	
	UPROPERTY(EditDefaultsOnly)
	ESpawnLocationType SpawnLocationType = ESpawnLocationType::SelectedPoint;
	
	UPROPERTY(EditDefaultsOnly)
	ESpawnerType SpawnerType = ESpawnerType::OnlyOnce;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<APointCollection> PointCollectionClass;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UBehaviourData> OverrideBehaviourData = nullptr;
};
