// @ Retropsis 2024-2025.

#include "World/Level/Spawner/EncounterSpawnVolume.h"
#include "PaperTileMapComponent.h"
#include "AbilitySystem/Actor/PointCollection.h"
#include "AI/AICharacter.h"
#include "Algo/RandomShuffle.h"
#include "Components/BoxComponent.h"
#include "Components/SplineComponent.h"
#include "Data/EncounterData.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"
#include "Interaction/PlayerInterface.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Leyr/Leyr.h"
#include "World/Level/Spawner/EncounterSpawnPointComponent.h"

AEncounterSpawnVolume::AEncounterSpawnVolume()
{
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(Root);
	
	TriggerVolume = CreateDefaultSubobject<UBoxComponent>("TriggerVolume");
	TriggerVolume->SetupAttachment(GetRootComponent());
	TriggerVolume->InitBoxExtent(FVector{ 100.f });
	TriggerVolume->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	TriggerVolume->SetCollisionResponseToAllChannels(ECR_Ignore);
	TriggerVolume->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	TriggerVolume->SetCollisionResponseToChannel(ECC_Player, ECR_Overlap);
	TriggerVolume->SetLineThickness(3.f);
	TriggerVolume->ShapeColor = FColor::Green;
	
	SpawningBounds = CreateDefaultSubobject<UBoxComponent>("SpawningBounds");
	SpawningBounds->SetupAttachment(GetRootComponent());
	SpawningBounds->InitBoxExtent(FVector{ 100.f });
	SpawningBounds->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SpawningBounds->SetCollisionResponseToAllChannels(ECR_Ignore);
	
	DespawningBounds = CreateDefaultSubobject<UBoxComponent>("DespawningBounds");
	DespawningBounds->SetupAttachment(GetRootComponent());
	DespawningBounds->InitBoxExtent(FVector{ 100.f });
	DespawningBounds->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	DespawningBounds->SetCollisionResponseToAllChannels(ECR_Ignore);
	DespawningBounds->SetCollisionResponseToChannel(ECC_Enemy, ECR_Overlap);
	DespawningBounds->SetLineThickness(3.f);
	DespawningBounds->ShapeColor = FColor::Red;
}

void AEncounterSpawnVolume::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	// if (IsValid(EncounterSpawnData) && IsValid(EncounterSpawnData->EncounterData) && IsValid(EncounterSpawnData->EncounterData->BehaviourData))
	// {
	// 	EncounterSpawnData->EncounterData->BehaviourData->OnMovementTypePropertyChanged.RemoveAll(EncounterSpawnData->EncounterData->BehaviourData);
	// 	EncounterSpawnData->EncounterData->BehaviourData->OnMovementTypePropertyChanged.AddLambda([&] (const EMovementType MovementType)
	// 	{
	// 		if (MovementType == EMovementType::Spline)
	// 		{
	// 			CreateSplineComponentActor();
	// 		}
	// 		else if (IsValid(SplineComponentActor))
	// 		{
	// 			SplineComponentActor->Destroy();
	// 			SplineComponentActor = nullptr;
	// 		}
	// 	});
	// }
}

void AEncounterSpawnVolume::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	// EncounterSpawnData
	if (PropertyChangedEvent.Property->GetName() == TEXT("EncounterSpawnData") && IsValid(EncounterSpawnData))
	{
		EncounterLevel = EncounterSpawnData->Level;
		Count = EncounterSpawnData->Count;
		RespawnTime = EncounterSpawnData->RespawnTime;
		PreferredSpawningRange = EncounterSpawnData->PreferredSpawningRange;
		SpawnerType = EncounterSpawnData->SpawnerType;
		SpawnLocationType = EncounterSpawnData->SpawnLocationType;
		CreateSpawnPoints();
	}
	// TileMap
	if (PropertyChangedEvent.Property->GetName() == TEXT("TileMap") && IsValid(TileMap))
	{
		TileMapBounds = TileMap->GetRenderComponent()->Bounds;
	}
}

void AEncounterSpawnVolume::LoadActor_Implementation()
{
	if (bActivated) DisableTriggerVolume();
}

void AEncounterSpawnVolume::CreateSpawnPoints()
{
	ClearSpawnPoints();
	
	if (IsValid(EncounterSpawnData->EncounterData))
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		
		for (int i = 0; i < Count; ++i)
		{
			CreateSpawnPoint(FVector{ 100.f + i * 50.f, 0.f, 0.f});
		}
	}
}

void AEncounterSpawnVolume::ClearSpawnPoints()
{
	if (SpawnPoints.Num() == 0) return;
	
	for (UEncounterSpawnPointComponent* SpawnPoint : SpawnPoints)
	{
		if (IsValid(SpawnPoint)) SpawnPoint->DestroyComponent();
	}
	SpawnPoints.Empty();
}

void AEncounterSpawnVolume::CreateSpawnPoint(const FVector& Offset)
{
	if (!IsValid(EncounterSpawnData->EncounterData))
	{
		UE_LOG(LogTemp, Error, TEXT("EncounterSpawnData EncounterData is NULL"));
		return;
	}
	
	if (!IsValid(EncounterSpawnPointComponentClass)) return;
	
	UEncounterSpawnPointComponent* SpawnPoint = NewObject<UEncounterSpawnPointComponent>(this, EncounterSpawnPointComponentClass);
	AddInstanceComponent(SpawnPoint);
	SpawnPoint->RegisterComponent();
	const FString NewLabel = EncounterSpawnData->EncounterData->GetName().Replace(TEXT("Encounter"), TEXT("SpawnPoint"));
	SpawnPoint->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName(*NewLabel));
	SpawnPoint->SetRelativeLocation(Offset);
	SpawnPoint->SetSprite(EncounterSpawnData->EncounterData->EncounterIcon);
	SpawnPoints.Add(SpawnPoint);
}

void AEncounterSpawnVolume::UpdateSpawnPoints()
{
	if (SpawnPoints.IsEmpty())
	{
		CreateSpawnPoints();
		return;
	}

	if (SpawnPoints.Num() != Count)
	{
		if (SpawnPoints.Num() > Count)
		{
			for (int i = Count - 1; i < SpawnPoints.Num(); ++i)
			{
				if (SpawnPoints.IsValidIndex(i))
				{
					SpawnPoints[i]->DestroyComponent();
					SpawnPoints.RemoveAt(i);
				}
			}
		}
		if (SpawnPoints.Num() < Count)
		{
			for (int i = SpawnPoints.Num(); i < Count; ++i)
			{
				// Create a new spawn point
				FActorSpawnParameters SpawnParams;
				SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
				FVector Offset = FVector{ 100.f + 50.f * i, 0.f, 0.f};
				CreateSpawnPoint(Offset);
			}
		}
		return;
	}
	
	if (!IsValid(EncounterSpawnData->EncounterData))
	{
		UE_LOG(LogTemp, Error, TEXT("EncounterSpawnData.EncounterData is NULL"));
		return;
	}
	
	for (UEncounterSpawnPointComponent* SpawnPoint : SpawnPoints)
	{
		SpawnPoint->SetSprite(EncounterSpawnData->EncounterData->EncounterIcon);
	}
}

void AEncounterSpawnVolume::AddSpawnPoint()
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	CreateSpawnPoint(FVector{ 100.f + SpawnPoints.Num() * 50.f, 0.f, 0.f});
}

void AEncounterSpawnVolume::RemoveSpawnPointAboveCount()
{
	if (SpawnPoints.Num() > Count && Count > 0)
	{
		UEncounterSpawnPointComponent* SpawnPoint = SpawnPoints.Last();
		SpawnPoints.RemoveAt(SpawnPoints.Num() - 1);
		if (IsValid(SpawnPoint)) SpawnPoint->DestroyComponent();
	}
}

void AEncounterSpawnVolume::SetAllBoundariesToRoomSize() const
{
	SetTriggerBoundaryToRoomSize();
	SetSpawnBoundaryToRoomSize();
	SetDespawnBoundaryToRoomSize();
}

void AEncounterSpawnVolume::SetTriggerBoundaryToRoomSize() const
{
	TriggerVolume->SetWorldLocation(TileMapBounds.Origin);
	TriggerVolume->SetBoxExtent(FVector{ TileMapBounds.BoxExtent.X - 16.f, 100.f, TileMapBounds.BoxExtent.Z - 16.f });
}

void AEncounterSpawnVolume::SetSpawnBoundaryToRoomSize() const
{
	SpawningBounds->SetWorldLocation(TileMapBounds.Origin);
	SpawningBounds->SetBoxExtent(FVector{ TileMapBounds.BoxExtent.X + 16.f, 100.f, TileMapBounds.BoxExtent.Z + 16.f });
}

void AEncounterSpawnVolume::SetDespawnBoundaryToRoomSize() const
{
	DespawningBounds->SetWorldLocation(TileMapBounds.Origin);
	DespawningBounds->SetBoxExtent(FVector{ TileMapBounds.BoxExtent.X + 16.f, 100.f, TileMapBounds.BoxExtent.Z + 16.f });
}

void AEncounterSpawnVolume::BeginPlay()
{
	Super::BeginPlay();
	if (HasAuthority())
	{
		TriggerVolume->OnComponentBeginOverlap.AddUniqueDynamic(this, &AEncounterSpawnVolume::OnBeginOverlap);
		DespawningBounds->OnComponentEndOverlap.AddUniqueDynamic(this, &AEncounterSpawnVolume::OnDespawnOverlap);
	}
}

void AEncounterSpawnVolume::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor && OtherActor->Implements<UPlayerInterface>())
	{
		PreferredLocation = FVector{ OtherActor->GetActorLocation().X, 0.f, GetActorLocation().Z };
		UKismetSystemLibrary::DrawDebugPoint(this, SpawningBounds->Bounds.Origin - FVector{ SpawningBounds->Bounds.BoxExtent.X, 0.f, 0.f }, 25.f, FLinearColor::Green, 90.f);
		UKismetSystemLibrary::DrawDebugPoint(this, SpawningBounds->Bounds.Origin + FVector{ SpawningBounds->Bounds.BoxExtent.X, 0.f, 0.f }, 25.f, FLinearColor::Green, 90.f);
		Target = OtherActor;
		SpawnEncounterGroup();
		if (SpawnerType == ESpawnerType::OnlyOnce) bActivated = true;
		DisableTriggerVolume();
	}
}

void AEncounterSpawnVolume::OnDespawnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(AAICharacter* Encounter = Cast<AAICharacter>(OtherActor))
	{
		for (UEncounterSpawnPointComponent* SpawnPoint : SpawnPoints)
		{
			if (!ICombatInterface::Execute_IsDefeated(Encounter))
			{
				bool bHasSpawned = RequestRespawnEncounter(Encounter);
				if (Encounter->IsActorBeingDestroyed())
				{
					// GEngine->AddOnScreenDebugMessage(-1, 8.f, FColor::Red, FString::Printf(TEXT("%s is being destroyed"), *Encounter->GetName()));
				}
				// Encounter->Destroy();
				// TODO: Meditate on this, other part is in ABaseCharacter::HandleDeathCapsuleComponent, Capsule deactivated triggers this too soon
				Encounter->SetLifeSpan(Encounter->LifeSpan);
				return;
			}
		}
	}
}

/*
 * Called by CameraBoundary when player enters CameraBounds
 */
void AEncounterSpawnVolume::HandlePlayerEntering()
{
	ToggleOnDespawnOverlap(true);
}

/*
 * Called by CameraBoundary when player leaves CameraBounds
 */
void AEncounterSpawnVolume::HandlePlayerLeaving()
{
	EnableTriggerVolume();
	ToggleOnDespawnOverlap(false);
	DespawnEncounter();
}

FBoundLocations AEncounterSpawnVolume::CalculateSpawningBounds() const
{
	FBoundLocations Bounds;
	const FVector Extent = SpawningBounds->GetScaledBoxExtent();
	const FVector Location = SpawningBounds->GetComponentLocation();
	Bounds.Left = FVector{ Location.X - Extent.X, 0.f, Location.Z };
	Bounds.Right = FVector{ Location.X + Extent.X, 0.f, Location.Z };
	return Bounds;
}

void AEncounterSpawnVolume::DisableTriggerVolume() const
{
	TriggerVolume->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	TriggerVolume->SetCollisionResponseToAllChannels(ECR_Ignore);
}

void AEncounterSpawnVolume::EnableTriggerVolume() const
{
	TriggerVolume->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	TriggerVolume->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	TriggerVolume->SetCollisionResponseToChannel(ECC_Player, ECR_Overlap);
}

void AEncounterSpawnVolume::ToggleOnDespawnOverlap(const bool bEnable) const
{
	if (bEnable)
	{
		DespawningBounds->OnComponentEndOverlap.AddUniqueDynamic(this, &AEncounterSpawnVolume::OnDespawnOverlap);
	}
	else
	{
		DespawningBounds->OnComponentEndOverlap.RemoveAll(this);
	}
}

void AEncounterSpawnVolume::CreateSplineComponent()
{
	if (!IsValid(EncounterSpawnData))
	{
		UE_LOG(LogTemp, Error, TEXT("EncounterSpawnData is NULL"));
		return;
	}
	if (!IsValid(EncounterSpawnData->EncounterData))
	{
		UE_LOG(LogTemp, Error, TEXT("EncounterSpawnData EncounterData is NULL"));
		return;
	}
	if (IsValid(SplineComponent))
	{
		UE_LOG(LogTemp, Warning, TEXT("Spline already exists"));
		return;
	}
	
	SplineComponent = NewObject<USplineComponent>(this);
	const FString NewLabel = EncounterSpawnData->EncounterData->GetName().Replace(TEXT("Encounter"), TEXT("Spline"));
	SplineComponent->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform, FName(*NewLabel));
	SplineComponent->SetRelativeLocation(FVector(0.f, 0.f, 100.f));
	for (int i = 0; i < SplineComponent->GetNumberOfSplinePoints(); ++i)
	{
		SplineComponent->SetSplinePointType(i, ESplinePointType::Linear);
	}
	SplineComponent->RegisterComponent();
	
}

void AEncounterSpawnVolume::RemoveSplineComponent() const
{
	if (IsValid(SplineComponent))
	{
		SplineComponent->DestroyComponent();
	}
}

/*
 * Spawning Encounters 
 */
void AEncounterSpawnVolume::SpawnEncounterGroup()
{
	if (EncounterSpawnData->EncounterData->EncounterClass.Get() == nullptr)
	{
		UAssetManager::GetStreamableManager().RequestAsyncLoad(EncounterSpawnData->EncounterData->EncounterClass.ToSoftObjectPath(), [this] ()
		{
			if (IsValid(EncounterSpawnData->EncounterData->EncounterClass.Get()))
			{
				SpawnEncountersDelayed();
			}
		}, FStreamableManager::AsyncLoadHighPriority);
	}
	else SpawnEncountersDelayed();
}

void AEncounterSpawnVolume::SpawnEncountersDelayed()
{
	TArray<int32> Indexes;
	for (int i = 0; i < SpawnPoints.Num(); ++i)
	{
		Indexes.Add(i);
	}
	Algo::RandomShuffle(Indexes);
	for (int i = 0; i < Count; ++i)
	{
		GetSpawnLocationsFromPointCollection();
		FTimerHandle SpawnTimer;
		int32 RandomIndex = Indexes.Last();
		Indexes.RemoveAt(Indexes.Num() - 1);
		GetWorld()->GetTimerManager().SetTimer(SpawnTimer, FTimerDelegate::CreateLambda([this, RandomIndex, SpawnTimer] ()
		{
			SpawnEncounter(EncounterSpawnData->EncounterData->EncounterClass.Get(), DetermineSpawnTransform(RandomIndex));
			SpawnTimers.Remove(SpawnTimer);
		}), SpawnDelay * (i + 1), false);
		SpawnTimers.Add(SpawnTimer);
	}
}

void AEncounterSpawnVolume::SpawnEncounter(UClass* EncounterToSpawn, const FTransform& InSpawnTransform)
{
	AAICharacter* Encounter = GetWorld()->SpawnActorDeferred<AAICharacter>(EncounterToSpawn, InSpawnTransform, this, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	Encounter->SetEncounterLevel(EncounterLevel);
	Encounter->SetEncounterData(EncounterSpawnData->EncounterData);
	if (OverrideBehaviourData) Encounter->SetBehaviourData(OverrideBehaviourData);

	FBoundLocations BoundLocations;
	BoundLocations.Left = SpawningBounds->Bounds.Origin - FVector{ SpawningBounds->Bounds.BoxExtent.X, 0.f, 0.f };
	BoundLocations.Right = SpawningBounds->Bounds.Origin + FVector{ SpawningBounds->Bounds.BoxExtent.X, 0.f, 0.f };
	IAIInterface::Execute_SetSpawningBounds(Encounter, BoundLocations);
	
	if (IsValid(SplineComponent)) Encounter->SplineComponent = SplineComponent;
	Encounter->SpawnDefaultController();
	Encounter->FinishSpawning(InSpawnTransform);
	CurrentSpawns.Add(Encounter);

	if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(Encounter); CombatInterface && SpawnerType == ESpawnerType::Infinite)
	{
		CombatInterface->GetOnDeath().AddDynamic(this, &AEncounterSpawnVolume::Respawn);
	}
}

void AEncounterSpawnVolume::Respawn(AActor* DefeatedEncounter)
{
	CurrentSpawns.Remove(DefeatedEncounter);
	if(SpawnerType == ESpawnerType::Infinite) {}
	
	FTimerHandle RespawnTimer;
	GetWorldTimerManager().SetTimer(RespawnTimer, [this] ()
	{
		const int32 RandomIndex = FMath::RandRange(0, SpawnPoints.Num() - 1);
		SpawnEncounter(EncounterSpawnData->EncounterData->EncounterClass.Get(), DetermineSpawnTransform(RandomIndex));
	}, RespawnTime, false);
	SpawnTimers.Add(RespawnTimer);
}

void AEncounterSpawnVolume::ClearSpawnTimers()
{
	for (FTimerHandle RespawnTimer : SpawnTimers)
	{
		GetWorldTimerManager().ClearTimer(RespawnTimer);
		RespawnTimer.Invalidate();
	}
	SpawnTimers.Empty();
}

/*
 * Despawn encounter when player leaves
 * (not killed, it doesn't respawn and timer is invalidated)
 */
void AEncounterSpawnVolume::DespawnEncounter()
{
	for (int i = 0; i < CurrentSpawns.Num(); ++i)
	{
		if (CurrentSpawns[i] != nullptr)
		{
			CurrentSpawns[i]->Destroy();
			CurrentSpawns.Shrink();
		}
	}
	ClearSpawnTimers();
}

bool AEncounterSpawnVolume::RequestRespawnEncounter(AAICharacter* Encounter)
{
	if (IsValid(Encounter) && CurrentSpawns.Contains(Encounter))
	{
		Respawn(Encounter);
		return true;
	}
	return false;
}

void AEncounterSpawnVolume::GetSpawnLocationsFromPointCollection()
{
	if (IsValid(PointCollectionClass))
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		APointCollection* PointCollection = GetWorld()->SpawnActor<APointCollection>(PointCollectionClass, GetActorTransform(), SpawnParams);
		SpawnLocations.Empty();
		const FVector SpawnBoundsOrigin = SpawningBounds->GetComponentLocation();
		if (Target.IsValid())
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

FTransform AEncounterSpawnVolume::DetermineSpawnTransform(const int32 SpawnLocationIndex)
{
	FTransform SpawnTransform = GetActorTransform();
	const FBoundLocations BoundLocations = CalculateSpawningBounds();
	
	switch (SpawnLocationType) {
	case ESpawnLocationType::SelectedPoint:
		if (SpawnPoints.IsValidIndex(SpawnLocationIndex) && IsValid(SpawnPoints[SpawnLocationIndex]))
		{
			SpawnTransform.SetLocation(SpawnPoints[SpawnLocationIndex]->GetComponentLocation());
			SpawnTransform.SetRotation(SpawnPoints[SpawnLocationIndex]->GetComponentRotation().Quaternion());
		}
		break;
	case ESpawnLocationType::SelectedPointAroundPlayer:
		if (Target.IsValid())
		{
			if (const TArray<UEncounterSpawnPointComponent*> SelectedSpawnPoints = FindSelectedPointAroundPlayer(Target->GetActorLocation()); SelectedSpawnPoints.Num() > 0)
			{
				const int32 SelectedIndex = SelectedSpawnPoints.Num() >= 2 ? FMath::RandRange(0, 1) : 0;
				const UEncounterSpawnPointComponent* SelectedSpawnPoint = SelectedSpawnPoints[SelectedIndex];
				
				SpawnTransform.SetLocation(SelectedSpawnPoint->GetComponentLocation());
				SpawnTransform.SetRotation(SelectedSpawnPoint->GetComponentRotation().Quaternion());
				break;
			}
		}
		SpawnTransform.SetLocation(SpawnPoints[SpawnLocationIndex]->GetComponentLocation());
		SpawnTransform.SetRotation(SpawnPoints[SpawnLocationIndex]->GetComponentRotation().Quaternion());
		break;
	case ESpawnLocationType::PointCollection:
		SpawnTransform.SetLocation(SpawnLocations[SpawnLocationIndex] + FVector{ 0.f, 0.f, 75.f });
		break;
	case ESpawnLocationType::PointCollectionRandom:
		SpawnTransform.SetLocation(SpawnLocations[FMath::RandRange(0, SpawnLocations.Num() - 1)] + FVector{ 0.f, 0.f, 75.f });
		break;
	case ESpawnLocationType::Random:
		SpawnTransform.SetLocation(FVector{ FMath::FRandRange(BoundLocations.Left.X, BoundLocations.Right.X), 0.f, BoundLocations.Left.Z });
		break;
	case ESpawnLocationType::RandomCloseToPlayer:
		SpawnTransform.SetLocation(FindRandomPointWithinBounds(PreferredLocation));
		break;
	case ESpawnLocationType::OutOfBounds:
		if (Target.IsValid())
		{
			const FVector SpawnBoundsOrigin = (BoundLocations.Left + BoundLocations.Right) / 2.f;
			if (SpawnBoundsOrigin.X < Target->GetActorLocation().X)
			{
				const FVector LeftBound{ BoundLocations.Left.X - 50.f, 0.f, Target->GetActorLocation().Z };
				UKismetSystemLibrary::DrawDebugSphere(this, LeftBound, 25.f, 20, FColor::Red, 90.f);
				UKismetSystemLibrary::DrawDebugSphere(this, GetActorLocation(), 25.f, 20, FColor::White, 90.f);
				SpawnTransform.SetLocation(LeftBound);
			}
			else
			{
				const FVector RightBound{ BoundLocations.Right.X + 50.f, 0.f, Target->GetActorLocation().Z };
				UKismetSystemLibrary::DrawDebugSphere(this, RightBound, 25.f, 20, FColor::Red, 90.f);
				UKismetSystemLibrary::DrawDebugSphere(this, GetActorLocation(), 25.f, 20, FColor::White, 90.f);
				SpawnTransform.SetLocation(RightBound);
			}
		}
		break;
	case ESpawnLocationType::OutOfBoundsFixed:
		break;
	}
	return SpawnTransform;
}

FVector AEncounterSpawnVolume::FindRandomPointWithinBounds(const FVector& Origin) const
{	
	FBoundLocations BoundLocations = CalculateSpawningBounds();
	const float PreferredLeftX = FMath::Clamp(Origin.X - PreferredSpawningRange, BoundLocations.Left.X, BoundLocations.Right.X);
	const float PreferredRightX = FMath::Clamp(Origin.X + PreferredSpawningRange, BoundLocations.Left.X, BoundLocations.Right.X);
	const bool bSweepLeftToRight = Origin.X > (BoundLocations.Left.X +  BoundLocations.Right.X) / 2.f;

	const float Delta = PreferredRightX - PreferredLeftX;
	const int32 MaxTries = static_cast<int32>(Delta / 80.f);
	FVector ChosenLocation = FVector::ZeroVector;
	int32 Tries = 0;
	while (Tries < MaxTries)
	{
		ChosenLocation = bSweepLeftToRight ? FVector{ PreferredLeftX + Tries * 80.f, 0.f, BoundLocations.Left.Z } : FVector{ PreferredRightX - Tries * 80.f, 0.f, BoundLocations.Left.Z };
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

TArray<UEncounterSpawnPointComponent*> AEncounterSpawnVolume::FindSelectedPointAroundPlayer(const FVector& PlayerLocation) const
{
	// float ClosestDistance = FLT_MAX;
	TMap<float, UEncounterSpawnPointComponent*> ChosenSpawnPoints;
	for (const TObjectPtr<UEncounterSpawnPointComponent>& SpawnPoint : SpawnPoints)
	{
		const float DistanceToPlayer = FVector::Distance(PlayerLocation, SpawnPoint->GetComponentLocation());
		if (DistanceToPlayer < 150.f) continue;
		
		ChosenSpawnPoints.Add(DistanceToPlayer, SpawnPoint);
	}
	ChosenSpawnPoints.KeySort([] (const float A, const float B)
	{
		return A < B;
	});
	
	TArray<UEncounterSpawnPointComponent*> OutSpawnPoints;
	ChosenSpawnPoints.GenerateValueArray(OutSpawnPoints);
	return OutSpawnPoints;
}