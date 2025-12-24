// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Data/EncounterSpawnData.h"
#include "GameFramework/Actor.h"
#include "Interaction/SaveInterface.h"
#include "Leyr/Leyr.h"
#include "EncounterSpawnVolume.generated.h"

class USplineComponent;
class UEncounterSpawnPointComponent;
class APaperTileMapActor;
class ASplineComponentActor;
class UEncounterSpawnData;
class AEncounterSpawnPoint;
class UBoxComponent;

DECLARE_MULTICAST_DELEGATE(FSplineComponentActorUpdated);

UCLASS()
class LEYR_API AEncounterSpawnVolume : public AActor, public ISaveInterface
{
	GENERATED_BODY()
	
public:	
	AEncounterSpawnVolume();
	virtual void OnConstruction(const FTransform& Transform) override;

	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
	
	void DisableTriggerVolume() const;
	void EnableTriggerVolume() const;
	void ToggleOnDespawnOverlap(bool bEnable) const;
	void CreateSpawnPoint(const FVector& Offset);
	void SpawnEncountersDelayed();
	void ClearSpawnPoints();
	void DespawnEncounter();

	//~ Save Interface
	virtual void LoadActor_Implementation() override;
	//~ Save Interface

	UFUNCTION(CallInEditor, Category="Spawner")
	void CreateSpawnPoints();
	
	UFUNCTION(CallInEditor, Category="Spawner")
	void UpdateSpawnPoints();
	
	UFUNCTION(CallInEditor, Category="Spawner")
	void AddSpawnPoint();
	
	UFUNCTION(CallInEditor, Category="Spawner")
	void RemoveSpawnPointAboveCount();
	
	UFUNCTION(CallInEditor, Category="Spawner")
	void SetAllBoundariesToRoomSize() const;
	
	UFUNCTION(CallInEditor, Category="Spawner")
	void SetTriggerBoundaryToRoomSize() const;
	
	UFUNCTION(CallInEditor, Category="Spawner")
	void SetSpawnBoundaryToRoomSize() const;
	
	UFUNCTION(CallInEditor, Category="Spawner")
	void SetDespawnBoundaryToRoomSize() const;
	
	UFUNCTION(CallInEditor, Category="Spawner")
	void CreateSplineComponent();
	
	UFUNCTION(CallInEditor, Category="Spawner")
	void RemoveSplineComponent() const;
	
	UFUNCTION()
	void HandlePlayerLeaving();
	
	UFUNCTION()
	void HandlePlayerEntering();

	/* GETTER SETTER */
	TArray<UEncounterSpawnPointComponent*> GetSpawnPoints() { return SpawnPoints; }
	void SetEncounterSpawnTag(const FGameplayTag& Tag) { EncounterSpawnTag = Tag; }
	FGameplayTag GetEncounterSpawnTag() const { return EncounterSpawnTag; }
	void SetTileMapBounds(const FBoxSphereBounds& Bounds) { TileMapBounds = Bounds; }

protected:
	virtual void BeginPlay() override;
	
	UFUNCTION()
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	virtual void OnDespawnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:
	FBoundLocations CalculateSpawningBounds() const;
	
	UPROPERTY(VisibleAnywhere, Category="Spawner")
	FGameplayTag EncounterSpawnTag = FGameplayTag::EmptyTag;
	
	UPROPERTY(EditDefaultsOnly, Category="Spawner")
	TSubclassOf<UEncounterSpawnPointComponent> EncounterSpawnPointComponentClass;
	
	UPROPERTY(VisibleAnywhere, Category="Spawner")
	TArray<TObjectPtr<UEncounterSpawnPointComponent>> SpawnPoints;
	
	UPROPERTY(VisibleAnywhere, Category="Spawner")
	TObjectPtr<USplineComponent> SplineComponent;

	UPROPERTY(EditAnywhere, Category="Spawner")
	TObjectPtr<APaperTileMapActor> TileMap;

	UPROPERTY(VisibleAnywhere, Category="Spawner")
	FBoxSphereBounds TileMapBounds;
	
	UPROPERTY(VisibleAnywhere, Category="Spawner")
	TObjectPtr<UBoxComponent> TriggerVolume;
	
	UPROPERTY(VisibleAnywhere, Category="Spawner")
	TObjectPtr<UBoxComponent> SpawningBounds;
	
	UPROPERTY(VisibleAnywhere, Category="Spawner")
	TObjectPtr<UBoxComponent> DespawningBounds;
	
	UPROPERTY(SaveGame)
	bool bActivated = false;

	/*
	 * Spawn point
	 */
	UFUNCTION(BlueprintCallable)
	void SpawnEncounterGroup();

	void SpawnEncounter(UClass* EncounterToSpawn, const FTransform& InSpawnTransform);

	void GetSpawnLocationsFromPointCollection();
	FTransform DetermineSpawnTransform(int32 SpawnLocationIndex = 0);

	UFUNCTION()
	void Respawn(AActor* DefeatedEncounter);
	void ClearSpawnTimers();

	bool RequestRespawnEncounter(AAICharacter* Encounter);

	FVector FindRandomPointWithinBounds(const FVector& Origin) const;
	
	FSplineComponentActorUpdated SplineComponentActorUpdated;
	
	UPROPERTY(EditAnywhere, Category="Spawner") TObjectPtr<UEncounterSpawnData> EncounterSpawnData = nullptr;
	UPROPERTY(EditAnywhere, Category="Spawner") int32 EncounterLevel = 1;
	UPROPERTY(EditAnywhere, Category="Spawner") int32 Count = 1;
	UPROPERTY(EditAnywhere, Category="Spawner") float RespawnTime = 120.f;
	UPROPERTY(EditAnywhere, Category="Spawner") float SpawnDelay = 1.f;
	UPROPERTY(EditAnywhere, Category="Spawner") float PreferredSpawningRange = 750.f;
	
	UPROPERTY() TSubclassOf<AAICharacter> EncounterClass;
	UPROPERTY() TObjectPtr<UBehaviourData> OverrideBehaviourData = nullptr;
	UPROPERTY() TSubclassOf<APointCollection> PointCollectionClass;
	UPROPERTY() ESpawnLocationType SpawnLocationType = ESpawnLocationType::SelectedPoint;
	UPROPERTY() ESpawnerType SpawnerType = ESpawnerType::OnlyOnce;
	
	FVector PreferredLocation = FVector::ZeroVector;
	
	TWeakObjectPtr<AActor> Target = nullptr;
	TArray<FVector> SpawnLocations;
	TArray<TWeakObjectPtr<AActor>> CurrentSpawns;
	TArray<FTimerHandle> SpawnTimers;
};
