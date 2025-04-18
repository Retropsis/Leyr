// @ Retropsis 2024-2025.

#include "World/Level/Spawner/EncounterSpawnPoint.h"
#include "AbilitySystem/Actor/PointCollection.h"
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
				TArray<FVector> SpawnLocations;
				if (PointCollectionClass && SpawnLocationType == ESpawnLocationType::PointCollection)
				{
					FActorSpawnParameters SpawnParams;
					SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
					APointCollection* PointCollection = GetWorld()->SpawnActor<APointCollection>(PointCollectionClass, GetActorTransform(), SpawnParams);
					SpawnLocations = PointCollection->GetGroundLocations(Count);
				}
				
				for (int i = 0; i < Count; ++i)
				{
					SpawnTransform = GetActorTransform();
					switch (SpawnLocationType) {
					case ESpawnLocationType::Point:
						break;
					case ESpawnLocationType::AroundPoint:
						SpawnTransform.SetLocation(FindRandomPointWithinBounds(GetActorLocation()));
						break;
					case ESpawnLocationType::PointCollection:
						SpawnTransform.SetLocation(SpawnLocations[i] + FVector{ 0.f, 0.f, 75.f });
						break;
					case ESpawnLocationType::Random:
						SpawnTransform.SetLocation(FVector{ FMath::FRandRange(SpawningBounds.Left.X, SpawningBounds.Right.X), 0.f, SpawningBounds.Left.Z });
						break;
					case ESpawnLocationType::RandomCloseToPlayer:
						SpawnTransform.SetLocation(FindRandomPointWithinBounds(PreferredLocation));
						break;
					}

					// Spawn An Encounter
					AAICharacter* Encounter = GetWorld()->SpawnActorDeferred<AAICharacter>(LoadedAsset, SpawnTransform, this, nullptr,
						ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);
					Encounter->SetEncounterLevel(EncounterLevel);
					Encounter->SetEncounterData(EncounterData);
					IAIInterface::Execute_SetSpawningBounds(Encounter, SpawningBounds);
					Encounter->FinishSpawning(SpawnTransform);
					Encounter->SpawnDefaultController();
					CurrentSpawns.Add(Encounter);

					if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(Encounter); CombatInterface && SpawnerType == ESpawnerType::Infinite)
					{
						CurrentCount++;
						CombatInterface->GetOnDeath().AddDynamic(this, &AEncounterSpawnPoint::Respawn);
					}
				}
			}
		}, FStreamableManager::AsyncLoadHighPriority);
	}
}

void AEncounterSpawnPoint::Respawn(AActor* DefeatedEncounter)
{
	CurrentCount--;
	if(CurrentCount <= 0)
	{
		GetWorldTimerManager().SetTimer(RespawnTimer, [this] (){ SpawnEncounter(); }, RespawnTime, false);
		SpawnTransform.SetLocation(FindRandomPointWithinBounds(GetActorLocation()));
	}
}

void AEncounterSpawnPoint::DespawnEncounter()
{
	if (CurrentSpawns.Num() > 0)
	{
		for (TObjectPtr<AActor> CurrentSpawn : CurrentSpawns)
		{
			if (IsValid(CurrentSpawn))
			{
				CurrentSpawn->Destroy();
				CurrentSpawn = nullptr;
			}
		}
	}
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
