// @ Retropsis 2024-2025.

#include "World/Level/Spawner/EncounterSpawnPoint.h"
#include "AbilitySystem/Actor/PointCollection.h"
#include "AI/AICharacter.h"
#include "Components/BillboardComponent.h"
#include "Data/EncounterData.h"
#include "Engine/AssetManager.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Player/PlayerCharacter.h"

AEncounterSpawnPoint::AEncounterSpawnPoint()
{
	PrimaryActorTick.bCanEverTick = false;
	GetSpriteComponent()->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f));
}

void AEncounterSpawnPoint::SpawnEncounter()
{
	if (Target && SpawnLocationType == ESpawnLocationType::OutOfBounds)
	{
		SetActorLocation(FVector{ GetActorLocation().X, 0.f, Target->GetActorLocation().Z });
	}
	
	if (EncounterData->EncounterClass)
	{
		UAssetManager::GetStreamableManager().RequestAsyncLoad(EncounterData->EncounterClass.ToSoftObjectPath(), [this] ()
		{
			const TSubclassOf<AAICharacter> LoadedAsset = EncounterData->EncounterClass.Get();
			if (IsValid(LoadedAsset))
			{
				TArray<FVector> SpawnLocations;
				if (PointCollectionClass)
				{
					FActorSpawnParameters SpawnParams;
					SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
					APointCollection* PointCollection = GetWorld()->SpawnActor<APointCollection>(PointCollectionClass, GetActorTransform(), SpawnParams);
					// if (SpawnLocationType == ESpawnLocationType::PointCollection)
					// {
					// 	SpawnLocations = PointCollection->GetGroundLocations(Count);
					// }
					if (Target && (SpawnLocationType == ESpawnLocationType::PointCollectionRandom || SpawnLocationType == ESpawnLocationType::PointCollection))
					{
						PointCollection->SetActorLocation(FVector{ PointCollection->GetActorLocation().X, 0.f, Target->GetActorLocation().Z });

						FName Tag;
						if (Target)
						{
							Tag = Target->GetActorLocation().X > PointCollection->GetActorLocation().X ? FName("Left") : FName("Right");
						}
						SpawnLocations = PointCollection->GetLocationsWithTag(Tag);
						PointCollection->Destroy();
					}
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
						UKismetSystemLibrary::DrawDebugSphere(this, SpawnTransform.GetLocation(), 18.f, 12, FLinearColor::White, 12.f);
						break;
					case ESpawnLocationType::PointCollectionRandom:
						SpawnTransform.SetLocation(SpawnLocations[FMath::RandRange(0, SpawnLocations.Num() - 1)]);
						break;
					case ESpawnLocationType::Random:
						SpawnTransform.SetLocation(FVector{ FMath::FRandRange(SpawningBounds.Left.X, SpawningBounds.Right.X), 0.f, SpawningBounds.Left.Z });
						break;
					case ESpawnLocationType::RandomCloseToPlayer:
						SpawnTransform.SetLocation(FindRandomPointWithinBounds(PreferredLocation));
						break;
					case ESpawnLocationType::OutOfBounds:
						break;
					case ESpawnLocationType::OutOfBoundsFixed:
						break;
					}

					// Spawn An Encounter
					AAICharacter* Encounter = GetWorld()->SpawnActorDeferred<AAICharacter>(LoadedAsset, SpawnTransform, this, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
					Encounter->SetEncounterLevel(EncounterLevel);
					Encounter->SetEncounterData(EncounterData);
					if (OverrideBehaviourData) Encounter->SetBehaviourData(OverrideBehaviourData);
					IAIInterface::Execute_SetSpawningBounds(Encounter, SpawningBounds);					
					Encounter->SpawnDefaultController();
					Encounter->FinishSpawning(SpawnTransform);
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
	CurrentSpawns.Remove(DefeatedEncounter);
	if(CurrentCount <= 0 && SpawnerType == ESpawnerType::Infinite)
	{
		GetWorldTimerManager().SetTimer(RespawnTimer, [this] (){ SpawnEncounter(); }, RespawnTime, false);
		SpawnTransform.SetLocation(FindRandomPointWithinBounds(GetActorLocation()));
	}
}

/*
 * Despawn encounter when player leaves
 * (not killed, it doesn't respawn and timer is invalidated)
 */
void AEncounterSpawnPoint::DespawnEncounter()
{
	for (int i = 0; i < CurrentSpawns.Num(); ++i)
	{
		if (CurrentSpawns[i] != nullptr)
		{
			CurrentSpawns[i]->Destroy();
			CurrentSpawns.Shrink();
		}
	}
	GetWorldTimerManager().ClearTimer(RespawnTimer);
	RespawnTimer.Invalidate();
}

/*
 * Request to respawn
 */
bool AEncounterSpawnPoint::RequestRespawnEncounter(AAICharacter* Encounter)
{
	if (IsValid(Encounter) && CurrentSpawns.Contains(Encounter))
	{
		Respawn(Encounter);
		return true;
	}
	return false;
}

FVector AEncounterSpawnPoint::FindRandomPointWithinBounds(const FVector& Origin) const
{
	const float PreferredLeftX = FMath::Clamp(Origin.X - PreferredSpawningRange, SpawningBounds.Left.X, SpawningBounds.Right.X);
	const float PreferredRightX = FMath::Clamp(Origin.X + PreferredSpawningRange, SpawningBounds.Left.X, SpawningBounds.Right.X);
	const float RandomX = FMath::FRandRange(PreferredLeftX, PreferredRightX);
	return  FVector{ RandomX, 0.f, SpawningBounds.Left.Z };
}
