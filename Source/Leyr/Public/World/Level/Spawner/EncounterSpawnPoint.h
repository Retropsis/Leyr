// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "Data/EncounterSpawnData.h"
#include "Engine/TargetPoint.h"
#include "Leyr/Leyr.h"
#include "EncounterSpawnPoint.generated.h"

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

	UPROPERTY() ESpawnLocationType SpawnLocationType = ESpawnLocationType::Point;
	UPROPERTY() ESpawnerType SpawnerType = ESpawnerType::Once;
	UPROPERTY() FBoundLocations SpawningBounds;
	UPROPERTY() FVector LeftBound = FVector::ZeroVector;
	UPROPERTY() FVector RightBound = FVector::ZeroVector;
	UPROPERTY() FVector PreferredLocation = FVector::ZeroVector;
	UPROPERTY(EditDefaultsOnly) float PreferredSpawningRange = 750.f;

private:
	UPROPERTY()
	TObjectPtr<AActor> CurrentSpawn = nullptr;
	FTimerHandle RespawnTimer;
};
