// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "Data/EncounterSpawnData.h"
#include "GameFramework/Actor.h"
#include "Interaction/SaveInterface.h"
#include "Leyr/Leyr.h"
#include "EncounterSpawnVolume.generated.h"

class UEncounterSpawnData;
class AEncounterSpawnPoint;
class UBoxComponent;

UCLASS()
class LEYR_API AEncounterSpawnVolume : public AActor, public ISaveInterface
{
	GENERATED_BODY()
	
public:	
	AEncounterSpawnVolume();
	void DisableVolume() const;
	void EnableVolume() const;

	//~ Save Interface
	virtual void LoadActor_Implementation() override;
	//~ Save Interface

	UFUNCTION(CallInEditor)
	void InitializeSpawnPoints();

	UFUNCTION(CallInEditor)
	void UpdateSpawnPoints();
	
	UFUNCTION()
	void HandlePlayerLeaving();
	
	UFUNCTION()
	void HandlePlayerEntering();
	
	void ClearSpawnPoints();
	void SetEncounterSpawnData(UEncounterSpawnData* Data) { EncounterSpawnData = Data; }

protected:
	virtual void BeginPlay() override;
	
	UFUNCTION()
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	virtual void OnDespawnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(EditAnywhere, Category="Spawner")
	TObjectPtr<UEncounterSpawnData> EncounterSpawnData;
	
	UPROPERTY(EditAnywhere, Category="Spawner")
	TArray<AEncounterSpawnPoint*> SpawnPoints;
	
private:
	FBoundLocations CalculateBounds() const;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBoxComponent> TriggerVolume;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> TriggerVolumeVisualizer;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBoxComponent> SpawningBounds;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> SpawningBoundsVisualizer;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBoxComponent> DespawningBounds;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> DespawningBoundsVisualizer;
	
	UPROPERTY(SaveGame)
	bool bActivated = false;
	
	UPROPERTY() ESpawnLocationType SpawnLocationType = ESpawnLocationType::Point;;
	UPROPERTY() ESpawnerType SpawnerType = ESpawnerType::Once;
	float PreferredSpawningRange = 0.f;
};
