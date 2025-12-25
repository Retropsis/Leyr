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
	void DropSpawnPointToGround(UEncounterSpawnPointComponent* SpawnPoint);
	void DespawnEncounter();

	//~ Save Interface
	virtual void LoadActor_Implementation() override;
	//~ Save Interface

	UFUNCTION(CallInEditor, Category="Leyr|Spawner")
	void CreateSpawnPoints();
	
	UFUNCTION(CallInEditor, Category="Leyr|Spawner")
	void UpdateSpawnPoints();
	
	UFUNCTION(CallInEditor, Category="Leyr|Spawner")
	void AddSpawnPoint();
	
	UFUNCTION(CallInEditor, Category="Leyr|Spawner")
	void RepositionSpawnPoints();
	
	UFUNCTION(CallInEditor, Category="Leyr|Spawner")
	void DropSpawnPointsToGround();
	
	UFUNCTION(CallInEditor, Category="Leyr|Spawner")
	void RemoveSpawnPointAboveCount();
	
	UFUNCTION(CallInEditor, Category="Leyr|Spawner")
	void SetAllBoundariesToRoomSize() const;
	
	UFUNCTION(CallInEditor, Category="Leyr|Spawner")
	void SetTriggerBoundaryToRoomSize() const;
	
	UFUNCTION(CallInEditor, Category="Leyr|Spawner")
	void SetSpawnBoundaryToRoomSize() const;
	
	UFUNCTION(CallInEditor, Category="Leyr|Spawner")
	void SetDespawnBoundaryToRoomSize() const;
	
	UFUNCTION(CallInEditor, Category="Leyr|Spawner")
	void SetAllBoundariesToSize() const;
	
	UFUNCTION(CallInEditor, Category="Leyr|Spawner")
	void SetTriggerBoundaryToSize(const FVector2D& BoxExtent) const;
	
	UFUNCTION(CallInEditor, Category="Leyr|Spawner")
	void SetSpawnBoundaryToSize(const FVector2D& BoxExtent) const;
	
	UFUNCTION(CallInEditor, Category="Leyr|Spawner")
	void SetDespawnBoundaryToSize(const FVector2D& BoxExtent) const;
	
	UFUNCTION(CallInEditor, Category="Leyr|Spawner")
	void CreateSplineComponent();
	
	UFUNCTION(CallInEditor, Category="Leyr|Spawner")
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
	void SetTileMap(APaperTileMapActor* InTileMap) { TileMap = InTileMap; }

protected:
	virtual void BeginPlay() override;
	
	UFUNCTION()
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	virtual void OnDespawnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:
	FBoundLocations CalculateSpawningBounds() const;
	
	UPROPERTY(VisibleAnywhere, Category="Leyr|Spawner")
	FGameplayTag EncounterSpawnTag = FGameplayTag::EmptyTag;
	
	UPROPERTY(EditDefaultsOnly, Category="Leyr|Spawner")
	TSubclassOf<UEncounterSpawnPointComponent> EncounterSpawnPointComponentClass;
	
	UPROPERTY()
	TArray<TObjectPtr<UEncounterSpawnPointComponent>> SpawnPoints;
	
	UPROPERTY(VisibleAnywhere, Category="Leyr|Spawner")
	TObjectPtr<USplineComponent> SplineComponent;

	UPROPERTY(EditAnywhere, Category="Leyr|Spawner")
	TObjectPtr<APaperTileMapActor> TileMap;

	UPROPERTY() FBoxSphereBounds TileMapBounds;
	UPROPERTY() TObjectPtr<UBoxComponent> TriggerVolume;
	UPROPERTY() TObjectPtr<UBoxComponent> SpawningBounds;
	UPROPERTY() TObjectPtr<UBoxComponent> DespawningBounds;
	
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
	TArray<UEncounterSpawnPointComponent*> FindSelectedPointAroundPlayer(const FVector& PlayerLocation) const;
	
	FSplineComponentActorUpdated SplineComponentActorUpdated;
	
	UPROPERTY(EditAnywhere, Category="Leyr|Spawner") TObjectPtr<UEncounterSpawnData> EncounterSpawnData = nullptr;
	UPROPERTY(EditAnywhere, Category="Leyr|Spawner") int32 EncounterLevel = 1;
	UPROPERTY(EditAnywhere, Category="Leyr|Spawner") int32 Count = 1;
	UPROPERTY(EditAnywhere, Category="Leyr|Spawner") float RespawnTime = 120.f;
	UPROPERTY(EditAnywhere, Category="Leyr|Spawner") float SpawnDelay = 1.f;
	UPROPERTY(EditAnywhere, Category="Leyr|Spawner") float PreferredSpawningRange = 750.f;
	UPROPERTY(EditAnywhere, Category="Leyr|Spawner") FVector2D SpawnPointOffset{ 50.f, 0.f };
	UPROPERTY(EditAnywhere, Category="Leyr|Spawner") FVector2D DefaultBoundaryBoxExtent{ 512.f, 384.f };
	UPROPERTY(EditAnywhere, Category="Leyr|Spawner") bool  bDropSpawnPointToGround = false;
	
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
