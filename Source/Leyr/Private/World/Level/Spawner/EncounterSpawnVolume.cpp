// @ Retropsis 2024-2025.

#include "World/Level/Spawner/EncounterSpawnVolume.h"
#include "AI/AICharacter.h"
#include "AI/SplineComponentActor.h"
#include "Components/BoxComponent.h"
#include "Components/SplineComponent.h"
#include "Data/BehaviourData.h"
#include "Data/EncounterData.h"
#include "Interaction/PlayerInterface.h"
#include "World/Level/Spawner/EncounterSpawnPoint.h"
#include "Leyr/Leyr.h"

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

	if (IsValid(EncounterSpawnData.EncounterData) && IsValid(EncounterSpawnData.EncounterData->BehaviourData))
	{
		EncounterSpawnData.EncounterData->BehaviourData->OnMovementTypePropertyChanged.RemoveAll(EncounterSpawnData.EncounterData->BehaviourData);
		EncounterSpawnData.EncounterData->BehaviourData->OnMovementTypePropertyChanged.AddLambda([&] (const EMovementType MovementType)
		{
			if (MovementType == EMovementType::Spline)
			{
				CreateSplineComponentActor();
			}
			else if (IsValid(SplineComponentActor))
			{
				SplineComponentActor->Destroy();
				SplineComponentActor = nullptr;
			}
			for (AEncounterSpawnPoint* SpawnPoint : SpawnPoints)
			{
				if (IsValid(SpawnPoint) && IsValid(SplineComponentActor))
				{
					SpawnPoint->SplineComponentActor = SplineComponentActor;
				} 
				if (!IsValid(SpawnPoint))
				{
					UE_LOG(LogTemp, Error, TEXT("SpawnPoint is not valid"));
				}
				if (! IsValid(SplineComponentActor))
				{
					UE_LOG(LogTemp, Error, TEXT("SplineComponentActor is not valid"));
				}
					
			}
		});
	}
}

void AEncounterSpawnVolume::LoadActor_Implementation()
{
	if (bActivated) DisableVolume();
}

void AEncounterSpawnVolume::CreateSplineComponentActor()
{
	SplineComponentActor = NewObject<ASplineComponentActor>(this);
	FString NewLabel = EncounterSpawnData.EncounterData->GetName().Replace(TEXT("Encounter"), TEXT("Spline"));
	SplineComponentActor->SetActorLabel(NewLabel);
	SplineComponentActor->SetActorLocation(GetActorLocation() + FVector(0.f, 0.f, 50.f));
	for (int i = 0; i < SplineComponentActor->GetSplineComponent()->GetNumberOfSplinePoints(); ++i)
	{
		SplineComponentActor->GetSplineComponent()->SetSplinePointType(i, ESplinePointType::Linear);
	}
	SplineComponentActor->RegisterAllComponents();
}

void AEncounterSpawnVolume::InitializeSpawnPoints()
{
	ClearSpawnPoints();
	
	if (EncounterSpawnData.EncounterClass)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		bool bUniqueSpawnLocationType = EncounterSpawnData.SpawnLocationType == ESpawnLocationType::Point || EncounterSpawnData.SpawnLocationType == ESpawnLocationType::AroundPoint;
		
		FVector Offset = FVector{ 100.f, 0.f, 0.f};
		
		for (int i = 0; i < EncounterSpawnData.Count; ++i)
		{
			CreateSpawnPoint(SpawnParams, bUniqueSpawnLocationType, Offset, i == 0);
			Offset.X += 50.f;

			if (!bUniqueSpawnLocationType)
			{
				UE_LOG(LogTemp, Warning, TEXT("%s"), *UEnum::GetValueAsString(EncounterSpawnData.SpawnLocationType));
				return;
			}
		}
	}
}

void AEncounterSpawnVolume::CreateSpawnPoint(const FActorSpawnParameters& SpawnParams, const bool bUniqueSpawnLocationType, const FVector& Offset, const bool bFirstIndex)
{
	AEncounterSpawnPoint* SpawnPoint = GetWorld()->SpawnActor<AEncounterSpawnPoint>(AEncounterSpawnPoint::StaticClass(), GetActorLocation() + Offset, FRotator::ZeroRotator, SpawnParams);
			
	UpdateSpawnPointData(bUniqueSpawnLocationType, SpawnPoint);
	SpawnPoint->SplineComponentActor = SplineComponentActor;
	UpdateSpawnPointEncounterIcon(SpawnPoint);
	UpdateSpawnPointLabel(SpawnPoint);
	SpawnPoints.Add(SpawnPoint);
}

void AEncounterSpawnVolume::UpdateSpawnPointData(bool bUniqueSpawnLocationType, AEncounterSpawnPoint* SpawnPoint) const
{
	if(EncounterSpawnData.OverrideBehaviourData) EncounterSpawnData.EncounterData->BehaviourData = EncounterSpawnData.OverrideBehaviourData;
	SpawnPoint->EncounterClass = EncounterSpawnData.EncounterClass;
	SpawnPoint->EncounterLevel = EncounterSpawnData.Level;
	SpawnPoint->EncounterData = EncounterSpawnData.EncounterData;
	SpawnPoint->RespawnTime = EncounterSpawnData.RespawnTime;
	SpawnPoint->SpawnerType = EncounterSpawnData.SpawnerType;
	SpawnPoint->SpawnLocationType = EncounterSpawnData.SpawnLocationType;
	SpawnPoint->PreferredSpawningRange = EncounterSpawnData.PreferredSpawningRange;
	SpawnPoint->PointCollectionClass = EncounterSpawnData.PointCollectionClass;
	SpawnPoint->Count = bUniqueSpawnLocationType ? 1 : EncounterSpawnData.Count;
}

void AEncounterSpawnVolume::UpdateSpawnPointLabel(AEncounterSpawnPoint* SpawnPoint)
{
	FString NewLabel = SpawnPoint->EncounterData->GetName().Replace(TEXT("Encounter"), TEXT("SpawnPoint"));
	SpawnPoint->SetActorLabel(NewLabel);
}

void AEncounterSpawnVolume::UpdateSpawnPointEncounterIcon(const AEncounterSpawnPoint* SpawnPoint) const
{
	if (EncounterSpawnData.EncounterData->EncounterIcon)
	{
		SpawnPoint->SetEncounterIcon(EncounterSpawnData.EncounterData->EncounterIcon);
	}
}

void AEncounterSpawnVolume::UpdateSpawnPoints()
{
	// if (!IsValid(EncounterSpawnData.EncounterClass)) return;
	if (SpawnPoints.IsEmpty())
	{
		InitializeSpawnPoints();
		return;
	}

	if (SpawnPoints.Num() != EncounterSpawnData.Count)
	{
		if (SpawnPoints.Num() > EncounterSpawnData.Count)
		{
			for (int i = EncounterSpawnData.Count - 1; i < SpawnPoints.Num(); ++i)
			{
				if (SpawnPoints.IsValidIndex(i))
				{
					SpawnPoints[i]->Destroy();
					SpawnPoints.RemoveAt(i);
				}
			}
		}
		if (SpawnPoints.Num() < EncounterSpawnData.Count)
		{
			for (int i = SpawnPoints.Num(); i < EncounterSpawnData.Count; ++i)
			{
				// Create a new spawn point
				FActorSpawnParameters SpawnParams;
				SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
				bool bUniqueSpawnLocationType = EncounterSpawnData.SpawnLocationType == ESpawnLocationType::Point || EncounterSpawnData.SpawnLocationType == ESpawnLocationType::AroundPoint;
				FVector Offset = FVector{ 100.f + 50.f * i, 0.f, 0.f};
				CreateSpawnPoint(SpawnParams, bUniqueSpawnLocationType, Offset, i == 0);
			}
		}
		return;
	}
	
	bool bUniqueSpawnLocationType = EncounterSpawnData.SpawnLocationType == ESpawnLocationType::Point || EncounterSpawnData.SpawnLocationType == ESpawnLocationType::AroundPoint;
	
	for (int i = 0; i < EncounterSpawnData.Count; ++i)
	{
		AEncounterSpawnPoint* SpawnPoint = SpawnPoints[i];

		if (SpawnPoint == nullptr || !IsValid(EncounterSpawnData.EncounterData)) continue;
		
		UpdateSpawnPointData(bUniqueSpawnLocationType, SpawnPoint);
		SpawnPoint->SplineComponentActor = SplineComponentActor;
		UpdateSpawnPointEncounterIcon(SpawnPoint);
		
		if (!bUniqueSpawnLocationType) return;
	}
}

void AEncounterSpawnVolume::SetTriggerBoundaryToRoomSize() const
{
	TriggerVolume->SetWorldLocation(TileMapBounds.Origin);
	TriggerVolume->SetWorldScale3D(FVector{ TileMapBounds.BoxExtent.X / 100.f - 0.2f, 1.f, TileMapBounds.BoxExtent.Z / 100.f - 0.2f });
}

void AEncounterSpawnVolume::SetDespawnBoundaryToRoomSize() const
{
	DespawningBounds->SetWorldLocation(TileMapBounds.Origin);
	DespawningBounds->SetWorldScale3D(FVector{ TileMapBounds.BoxExtent.X / 100.f + 0.2f, 1.f, TileMapBounds.BoxExtent.Z / 100.f + 0.2f });
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
		for (AEncounterSpawnPoint* SpawnPoint : SpawnPoints)
		{
			if (IsValid(SpawnPoint))
			{
				SpawnPoint->PreferredLocation = FVector{ OtherActor->GetActorLocation().X, 0.f, GetActorLocation().Z };
				SpawnPoint->SpawningBounds = CalculateBounds();
				SpawnPoint->Target = OtherActor;
				SpawnPoint->SpawnEncounterGroup();
			}
		}
		if (SpawnerType == ESpawnerType::Once) bActivated = true;
		DisableVolume();
	}
}

void AEncounterSpawnVolume::OnDespawnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(AAICharacter* Encounter = Cast<AAICharacter>(OtherActor))
	{
		for (AEncounterSpawnPoint* SpawnPoint : SpawnPoints)
		{
			if (!ICombatInterface::Execute_IsDefeated(Encounter))
			{
				bool bHasSpawned = SpawnPoint->RequestRespawnEncounter(Encounter);
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

void AEncounterSpawnVolume::HandlePlayerLeaving()
{
	EnableVolume();
	ToggleOnDespawnOverlap(false);
	for (AEncounterSpawnPoint* SpawnPoint : SpawnPoints)
	{
		if (SpawnPoint) SpawnPoint->DespawnEncounter();
	}
}

void AEncounterSpawnVolume::HandlePlayerEntering()
{
	ToggleOnDespawnOverlap(true);
	// GEngine->AddOnScreenDebugMessage(-1, 8.f, FColor::Green, FString::Printf(TEXT("HandlePlayerEntering")));
}

void AEncounterSpawnVolume::ClearSpawnPoints()
{
	if (SpawnPoints.Num() == 0) return;
	
	for (AEncounterSpawnPoint* SpawnPoint : SpawnPoints)
	{
		if (IsValid(SpawnPoint)) SpawnPoint->Destroy();
	}
	SpawnPoints.Empty();
}

FBoundLocations AEncounterSpawnVolume::CalculateBounds() const
{
	FBoundLocations Bounds;
	const FVector Extent = SpawningBounds->GetScaledBoxExtent();
	const FVector Location = SpawningBounds->GetComponentLocation();
	Bounds.Left = FVector{ Location.X - Extent.X, 0.f, Location.Z };
	Bounds.Right = FVector{ Location.X + Extent.X, 0.f, Location.Z };
	return Bounds;
}

void AEncounterSpawnVolume::DisableVolume() const
{
	TriggerVolume->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	TriggerVolume->SetCollisionResponseToAllChannels(ECR_Ignore);
}

void AEncounterSpawnVolume::EnableVolume() const
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
