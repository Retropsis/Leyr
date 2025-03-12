// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interaction/SaveInterface.h"
#include "EncounterSpawnVolume.generated.h"

UENUM(BlueprintType)
enum class ESpawnerType : uint8
{
	None,
	Once,
	Infinite,
};

class AEncounterSpawnPoint;
class UBoxComponent;

UCLASS()
class LEYR_API AEncounterSpawnVolume : public AActor, public ISaveInterface
{
	GENERATED_BODY()
	
public:	
	AEncounterSpawnVolume();
	void DisableVolume() const;

	//~ Save Interface
	virtual void LoadActor_Implementation() override;
	//~ Save Interface

protected:
	virtual void BeginPlay() override;
	
	UFUNCTION()
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere, Category="Spawner")
	TArray<AEncounterSpawnPoint*> SpawnPoints;

	UPROPERTY(EditAnywhere, Category="Spawner")
	ESpawnerType SpawnerType = ESpawnerType::Once;
	
private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBoxComponent> BoxOverlap;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> BoxVisualizer;
	
	UPROPERTY(SaveGame)
	bool bActivated = false;
};
