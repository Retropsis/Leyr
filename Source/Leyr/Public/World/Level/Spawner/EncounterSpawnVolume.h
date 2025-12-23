// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Data/EncounterSpawnData.h"
#include "GameFramework/Actor.h"
#include "Interaction/SaveInterface.h"
#include "Leyr/Leyr.h"
#include "EncounterSpawnVolume.generated.h"

class ASplineComponentActor;
class UEncounterSpawnData;
class AEncounterSpawnPoint;
class UBoxComponent;

UCLASS()
class LEYR_API AEncounterSpawnVolume : public AActor, public ISaveInterface
{
	GENERATED_BODY()
	
public:	
	AEncounterSpawnVolume();
	virtual void OnConstruction(const FTransform& Transform) override;

	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
	
	void DisableVolume() const;
	void EnableVolume() const;
	void ToggleOnDespawnOverlap(bool bEnable) const;
	void CreateSpawnPoint(const FActorSpawnParameters& SpawnParams, bool bUniqueSpawnLocationType, const FVector& Offset, bool bFirstIndex);
	void UpdateSpawnPointData(bool bUniqueSpawnLocationType, AEncounterSpawnPoint* SpawnPoint) const;
	void UpdateSpawnPointLabel(AEncounterSpawnPoint* SpawnPoint);
	void UpdateSpawnPointEncounterIcon(const AEncounterSpawnPoint* SpawnPoint) const;
	void CreateSplineComponentActor();

	//~ Save Interface
	virtual void LoadActor_Implementation() override;
	//~ Save Interface

	UFUNCTION(CallInEditor, Category="Spawner")
	void CreateSpawnPoints();
	
	UFUNCTION(CallInEditor, Category="Spawner")
	void UpdateSpawnPoints();
	
	UFUNCTION(CallInEditor, Category="Spawner")
	void SetTriggerBoundaryToRoomSize() const;
	
	UFUNCTION(CallInEditor, Category="Spawner")
	void SetSpawnBoundaryToRoomSize() const;
	
	UFUNCTION(CallInEditor, Category="Spawner")
	void SetDespawnBoundaryToRoomSize() const;
	
	UFUNCTION()
	void HandlePlayerLeaving();
	
	UFUNCTION()
	void HandlePlayerEntering();
	
	void ClearSpawnPoints();

	/* GETTER SETTER */
	TArray<AEncounterSpawnPoint*> GetSpawnPoints() { return SpawnPoints; }
	void SetEncounterSpawnData(const FEncounterSpawn& Data) { EncounterSpawnData = Data; }
	void SetEncounterSpawnTag(const FGameplayTag& Tag) { EncounterSpawnTag = Tag; }
	FGameplayTag GetEncounterSpawnTag() const { return EncounterSpawnTag; }

protected:
	virtual void BeginPlay() override;
	
	UFUNCTION()
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	virtual void OnDespawnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	
private:
	FBoundLocations CalculateBounds() const;
	
	UPROPERTY(VisibleAnywhere, Category="Spawner")
	FGameplayTag EncounterSpawnTag = FGameplayTag::EmptyTag;
	
	UPROPERTY(EditAnywhere, Category="Spawner")
	FEncounterSpawn EncounterSpawnData;
	
	UPROPERTY(VisibleAnywhere, Category="Spawner")
	TArray<AEncounterSpawnPoint*> SpawnPoints;
	
	UPROPERTY(VisibleAnywhere, Category="Spawner")
	TObjectPtr<ASplineComponentActor> SplineComponentActor;

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
	
	UPROPERTY() ESpawnLocationType SpawnLocationType = ESpawnLocationType::Point;;
	UPROPERTY() ESpawnerType SpawnerType = ESpawnerType::Once;
	float PreferredSpawningRange = 0.f;
};
