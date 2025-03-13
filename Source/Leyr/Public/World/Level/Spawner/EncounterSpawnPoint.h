// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Data/EncounterSpawnData.h"
#include "Engine/TargetPoint.h"
#include "EncounterSpawnPoint.generated.h"

enum class ESpawnerType : uint8;
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
	UFUNCTION(BlueprintCallable)
	void SpawnEncounter();

	UFUNCTION()
	void Respawn(AActor* DefeatedEncounter);

	void DespawnEncounter();

	UPROPERTY()
	float RespawnTime = 120.f;
	
	UPROPERTY(EditAnywhere, Category="Spawner")
	TSubclassOf<AAICharacter> EncounterClass;
	
	UPROPERTY(EditAnywhere, Category="Spawner")
	UEncounterData* EncounterData;

	UPROPERTY(EditAnywhere, Category="Spawner")
	int32 EncounterLevel = 1;

	bool bRandomizeLocation;
	ESpawnerType SpawnerType = ESpawnerType::Once;
	FVector LeftBound = FVector::ZeroVector;
	FVector RightBound = FVector::ZeroVector;

private:
	UPROPERTY()
	TObjectPtr<AActor> CurrentSpawn = nullptr;
};
