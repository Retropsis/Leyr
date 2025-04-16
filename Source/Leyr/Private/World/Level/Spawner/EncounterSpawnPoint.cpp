// @ Retropsis 2024-2025.

#include "World/Level/Spawner/EncounterSpawnPoint.h"
#include "AI/AICharacter.h"
#include "Data/EncounterData.h"
#include "Engine/AssetManager.h"

AEncounterSpawnPoint::AEncounterSpawnPoint()
{
	PrimaryActorTick.bCanEverTick = false;
}

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
				if (SpawnLocationType == ESpawnLocationType::Random)
				{
					const float RandomX = FMath::FRandRange(SpawningBounds.Left.X, SpawningBounds.Right.X);
					const FVector RandomLocation = FVector{ RandomX, 0.f, SpawningBounds.Left.Z };
					SpawnTransform.SetLocation(RandomLocation);
				}
				if (SpawnLocationType == ESpawnLocationType::RandomCloseToPlayer)
				{
					SpawnTransform.SetLocation(FindRandomPointWithinBounds(PreferredLocation));
				}
				if (SpawnLocationType == ESpawnLocationType::AroundPoint)
				{
					SpawnTransform.SetLocation(FindRandomPointWithinBounds(GetActorLocation()));
				}
				
				AAICharacter* Encounter = GetWorld()->SpawnActorDeferred<AAICharacter>(LoadedAsset, SpawnTransform, this, nullptr,
					ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);
				Encounter->SetEncounterLevel(EncounterLevel);
				Encounter->SetEncounterData(EncounterData);
				IAIInterface::Execute_SetSpawningBounds(Encounter, SpawningBounds);
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
	GetWorldTimerManager().SetTimer(RespawnTimer, [this] (){ SpawnEncounter(); }, RespawnTime, false);
}

void AEncounterSpawnPoint::DespawnEncounter()
{
	if (IsValid(CurrentSpawn))
	{
		CurrentSpawn->Destroy();
	}
	CurrentSpawn = nullptr;
	GetWorldTimerManager().ClearTimer(RespawnTimer);
	RespawnTimer.Invalidate();
}

FVector AEncounterSpawnPoint::FindRandomPointWithinBounds(const FVector& Origin) const
{
	const float PreferredLeftX = FMath::Clamp(Origin.X - PreferredSpawningRange, SpawningBounds.Left.X, SpawningBounds.Right.X);
	const float PreferredRightX = FMath::Clamp(Origin.X + PreferredSpawningRange, SpawningBounds.Left.X, SpawningBounds.Right.X);
	const float RandomX = FMath::FRandRange(PreferredLeftX, PreferredRightX);
	return  FVector{ RandomX, 0.f, SpawningBounds.Left.Z };
}
