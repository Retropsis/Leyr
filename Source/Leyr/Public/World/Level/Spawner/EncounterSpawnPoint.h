// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "Data/EncounterSpawnData.h"
#include "Engine/TargetPoint.h"
#include "Leyr/Leyr.h"
#include "EncounterSpawnPoint.generated.h"

class APointCollection;
class UBehaviourData;
class UEncounterData;
class AAICharacter;
/**
 * 
 */
UCLASS()
class LEYR_API AEncounterSpawnPoint : public ATargetPoint
{
	GENERATED_BODY()

public:
	AEncounterSpawnPoint();
	
	UFUNCTION(BlueprintCallable)
	void SpawnEncounter();

	UFUNCTION()
	void Respawn(AActor* DefeatedEncounter);

	void DespawnEncounter();

	FVector FindRandomPointWithinBounds(const FVector& Origin) const;

	UPROPERTY()
	float RespawnTime = 120.f;
	
	UPROPERTY(EditAnywhere, Category="Spawner")
	TSubclassOf<AAICharacter> EncounterClass;
	
	UPROPERTY(EditAnywhere, Category="Spawner")
	UEncounterData* EncounterData = nullptr;

	UPROPERTY(EditAnywhere, Category="Spawner")
	int32 EncounterLevel = 1;
	
	UPROPERTY()
	TSubclassOf<APointCollection> PointCollectionClass;

	UPROPERTY() ESpawnLocationType SpawnLocationType = ESpawnLocationType::Point;
	UPROPERTY() ESpawnerType SpawnerType = ESpawnerType::Once;
	UPROPERTY() FBoundLocations SpawningBounds;
	UPROPERTY() int32 Count = 1;
	UPROPERTY() FVector LeftBound = FVector::ZeroVector;
	UPROPERTY() FVector RightBound = FVector::ZeroVector;
	UPROPERTY() FVector PreferredLocation = FVector::ZeroVector;
	UPROPERTY() FTransform SpawnTransform = FTransform::Identity;
	UPROPERTY(EditDefaultsOnly) float PreferredSpawningRange = 750.f;

private:
	UPROPERTY() TArray<TObjectPtr<AActor>> CurrentSpawns;
	FTimerHandle RespawnTimer;
	int32 CurrentCount = 0;
};
