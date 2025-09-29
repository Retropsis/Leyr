// @ Retropsis 2024-2025.

#include "World/Level/Spawner/EncounterSpawnPoint.h"
#include "AbilitySystem/Actor/PointCollection.h"
#include "AI/AICharacter.h"
#include "Algo/RandomShuffle.h"
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

void AEncounterSpawnPoint::SpawnEncounterGroup()
{	
	if (EncounterData->EncounterClass.Get() == nullptr)
	{
		UAssetManager::GetStreamableManager().RequestAsyncLoad(EncounterData->EncounterClass.ToSoftObjectPath(), [this] ()
		{
			const TSubclassOf<AAICharacter> LoadedAsset = EncounterData->EncounterClass.Get();
			if (IsValid(LoadedAsset))
			{
				for (int i = 0; i < Count; ++i)
				{
					GetSpawnLocations();
					FTimerHandle SpawnTimer;
					GetWorld()->GetTimerManager().SetTimer(SpawnTimer, FTimerDelegate::CreateLambda([this, LoadedAsset, i] ()
					{
						DetermineSpawnTransform(i);
						SpawnEncounter(LoadedAsset, SpawnTransform);
					}), SpawnDelay * (i + 1), false);
				}
			}
		}, FStreamableManager::AsyncLoadHighPriority);
	}
	else
	{
		for (int i = 0; i < Count; ++i)
		{
			GetSpawnLocations();
			FTimerHandle SpawnTimer;
			GetWorld()->GetTimerManager().SetTimer(SpawnTimer, FTimerDelegate::CreateLambda([this, i] ()
			{
				DetermineSpawnTransform(i);
				SpawnEncounter(EncounterData->EncounterClass.Get(), SpawnTransform);
			}), SpawnDelay * (i + 1), false);
		}
	}
}

void AEncounterSpawnPoint::SpawnEncounter(UClass* EncounterToSpawn, const FTransform& InSpawnTransform)
{
	// Spawn An Encounter
	AAICharacter* Encounter = GetWorld()->SpawnActorDeferred<AAICharacter>(EncounterToSpawn, InSpawnTransform, this, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	Encounter->SetEncounterLevel(EncounterLevel);
	Encounter->SetEncounterData(EncounterData);
	if (OverrideBehaviourData) Encounter->SetBehaviourData(OverrideBehaviourData);
	IAIInterface::Execute_SetSpawningBounds(Encounter, SpawningBounds);					
	Encounter->SpawnDefaultController();
	Encounter->FinishSpawning(InSpawnTransform);
	CurrentSpawns.Add(Encounter);

	if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(Encounter); CombatInterface && SpawnerType == ESpawnerType::Infinite)
	{
		CombatInterface->GetOnDeath().AddDynamic(this, &AEncounterSpawnPoint::Respawn);
	}
}

void AEncounterSpawnPoint::GetSpawnLocations()
{
	if (PointCollectionClass)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		APointCollection* PointCollection = GetWorld()->SpawnActor<APointCollection>(PointCollectionClass, GetActorTransform(), SpawnParams);
		// if (SpawnLocationType == ESpawnLocationType::PointCollection)
		// {
		// 	SpawnLocations = PointCollection->GetGroundLocations(Count);
		// }
		const FVector SpawnBoundsOrigin = ( SpawningBounds.Left + SpawningBounds.Right ) / 2.f;
		if (Target && (SpawnLocationType == ESpawnLocationType::PointCollectionRandom || SpawnLocationType == ESpawnLocationType::PointCollection))
		{
			FName Tag;	
			if (Target->GetActorLocation().X < SpawnBoundsOrigin.X)
			{
				PointCollection->SetActorLocation(FVector{ Target->GetActorLocation().X, 0.f, SpawnBoundsOrigin.Z });
				Tag = FName("Right");
			}
			else
			{
				PointCollection->SetActorLocation(FVector{ Target->GetActorLocation().X, 0.f, SpawnBoundsOrigin.Z });
				Tag = FName("Left");
			}
			
			SpawnLocations = PointCollection->GetLocationsWithTag(Tag);
			Algo::RandomShuffle(SpawnLocations);
			PointCollection->Destroy();
		}
	}
}

void AEncounterSpawnPoint::DetermineSpawnTransform(int32 SpawnLocationIndex)
{
	SpawnTransform = GetActorTransform();
	switch (SpawnLocationType) {
	case ESpawnLocationType::Point:
		break;
	case ESpawnLocationType::AroundPoint:
		SpawnTransform.SetLocation(FindRandomPointWithinBounds(GetActorLocation()));
		break;
	case ESpawnLocationType::PointCollection:
		SpawnTransform.SetLocation(SpawnLocations[SpawnLocationIndex] + FVector{ 0.f, 0.f, 75.f });
		break;
	case ESpawnLocationType::PointCollectionRandom:
		SpawnTransform.SetLocation(SpawnLocations[FMath::RandRange(0, SpawnLocations.Num() - 1)] + FVector{ 0.f, 0.f, 75.f });
		break;
	case ESpawnLocationType::Random:
		SpawnTransform.SetLocation(FVector{ FMath::FRandRange(SpawningBounds.Left.X, SpawningBounds.Right.X), 0.f, SpawningBounds.Left.Z });
		break;
	case ESpawnLocationType::RandomCloseToPlayer:
		SpawnTransform.SetLocation(FindRandomPointWithinBounds(PreferredLocation));
		break;
	case ESpawnLocationType::OutOfBounds:
		if (Target && SpawnLocationType == ESpawnLocationType::OutOfBounds)
		{
			SetActorLocation(FVector{ GetActorLocation().X, 0.f, Target->GetActorLocation().Z });
			SpawnTransform = GetActorTransform();
		}
		break;
	case ESpawnLocationType::OutOfBoundsFixed:
		break;
	}
}

void AEncounterSpawnPoint::Respawn(AActor* DefeatedEncounter)
{
	CurrentSpawns.Remove(DefeatedEncounter);
	if(SpawnerType == ESpawnerType::Infinite)
	{
		FTimerHandle RespawnTimer;
		RespawnTimers.Add(RespawnTimer);
		GetWorldTimerManager().SetTimer(RespawnTimer, [this] ()
		{
			// DetermineSpawnTransform();
			if (Target) SpawnTransform.SetLocation(FindRandomPointWithinBounds(Target->GetActorLocation()));
			else SpawnTransform.SetLocation(FindRandomPointWithinBounds(GetActorLocation()));
			SpawnEncounter(EncounterData->EncounterClass.Get(), SpawnTransform);
		}, RespawnTime, false);
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
	for (FTimerHandle RespawnTimer : RespawnTimers)
	{
		GetWorldTimerManager().ClearTimer(RespawnTimer);
		RespawnTimer.Invalidate();
	}
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
	const bool bSweepLeftToRight = Origin.X > (SpawningBounds.Left.X +  SpawningBounds.Right.X) / 2.f;

	const float Delta = PreferredRightX - PreferredLeftX;
	const int32 MaxTries = static_cast<int32>(Delta / 80.f);
	FVector ChosenLocation = FVector::ZeroVector;
	int32 Tries = 0;
	while (Tries < MaxTries)
	{
		ChosenLocation = bSweepLeftToRight ? FVector{ PreferredLeftX + Tries * 80.f, 0.f, SpawningBounds.Left.Z } : FVector{ PreferredRightX - Tries * 80.f, 0.f, SpawningBounds.Left.Z };
		// ChosenLocation = FVector{ FMath::FRandRange(PreferredLeftX, PreferredRightX), 0.f, SpawningBounds.Left.Z };
		FHitResult Hit;
		UKismetSystemLibrary::BoxTraceSingle(this, ChosenLocation - FVector{ 0.f, 0.f, 50.f }, ChosenLocation + FVector{ 0.f, 0.f, 50.f }, 
			{ 80.f, 50.f, 20.f }, FRotator::ZeroRotator,
			TraceTypeQuery3, false, TArray<AActor*>(), EDrawDebugTrace::None, Hit, true);
		if (!Hit.bBlockingHit) return ChosenLocation;
		Tries++;
	}
	return  ChosenLocation;
}
