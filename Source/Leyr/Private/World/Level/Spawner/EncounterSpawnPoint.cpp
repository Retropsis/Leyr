// @ Retropsis 2024-2025.

#include "World/Level/Spawner/EncounterSpawnPoint.h"
#include "AI/AICharacter.h"
#include "Data/EncounterData.h"
#include "Engine/AssetManager.h"

void AEncounterSpawnPoint::SpawnEncounter()
{
	if (EncounterData->EncounterClass)
	{
		UAssetManager::GetStreamableManager().RequestAsyncLoad(EncounterData->EncounterClass.ToSoftObjectPath(), [this] ()
		{
			const TSubclassOf<AAICharacter> LoadedAsset = EncounterData->EncounterClass.Get();
			if (IsValid(LoadedAsset))
			{
				FTransform SpawnTransform = GetActorTransform();
				if (bRandomizeLocation)
				{
					const float RandomX = FMath::FRandRange(LeftBound.X, RightBound.X);
					const FVector RandomLocation = FVector{ RandomX, 0.f, LeftBound.Z };
					SpawnTransform.SetLocation(RandomLocation);
				}
				
				AAICharacter* Encounter = GetWorld()->SpawnActorDeferred<AAICharacter>(LoadedAsset, SpawnTransform, this, nullptr,
					ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);
				Encounter->SetEncounterLevel(EncounterLevel);
				Encounter->SetEncounterData(EncounterData); 
				Encounter->FinishSpawning(SpawnTransform);
				Encounter->SpawnDefaultController();
				CurrentSpawn = Encounter;

				if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(Encounter); CombatInterface && SpawnerType == ESpawnerType::Infinite)
				{
					CombatInterface->GetOnDeath().AddDynamic(this, &AEncounterSpawnPoint::Respawn);
				}
			}
		}, FStreamableManager::AsyncLoadHighPriority);
	}
}

void AEncounterSpawnPoint::Respawn(AActor* DefeatedEncounter)
{
	FTimerHandle RespawnTimer;
	GetWorldTimerManager().SetTimer(RespawnTimer, [this] (){ SpawnEncounter(); }, RespawnTime, false);
}

void AEncounterSpawnPoint::DespawnEncounter()
{
	if (IsValid(CurrentSpawn))
	{
		CurrentSpawn->Destroy();
		CurrentSpawn = nullptr;
	}
}
