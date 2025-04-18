// @ Retropsis 2024-2025.

#include "World/Level/Spawner/EncounterSpawnVolume.h"
#include "Components/BoxComponent.h"
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
	
	TriggerVolumeVisualizer = CreateDefaultSubobject<UStaticMeshComponent>("TriggerVolumeVisualizer");
	TriggerVolumeVisualizer->SetupAttachment(TriggerVolume);
	TriggerVolumeVisualizer->SetWorldScale3D(FVector{ 2.f });
	TriggerVolumeVisualizer->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	TriggerVolumeVisualizer->SetHiddenInGame(true);
	TriggerVolumeVisualizer->SetGenerateOverlapEvents(false);
	
	SpawningBounds = CreateDefaultSubobject<UBoxComponent>("SpawningBounds");
	SpawningBounds->SetupAttachment(GetRootComponent());
	SpawningBounds->InitBoxExtent(FVector{ 100.f });
	SpawningBounds->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SpawningBounds->SetCollisionResponseToAllChannels(ECR_Ignore);
	
	SpawningBoundsVisualizer = CreateDefaultSubobject<UStaticMeshComponent>("SpawningBoundsVisualizer");
	SpawningBoundsVisualizer->SetupAttachment(SpawningBounds);
	SpawningBoundsVisualizer->SetWorldScale3D(FVector{ 2.f });
	SpawningBoundsVisualizer->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SpawningBoundsVisualizer->SetHiddenInGame(true);
	SpawningBoundsVisualizer->SetGenerateOverlapEvents(false);
}

void AEncounterSpawnVolume::LoadActor_Implementation()
{
	if (bActivated) DisableVolume();
}

void AEncounterSpawnVolume::InitializeSpawnPoints()
{
	for (AEncounterSpawnPoint* SpawnPoint : SpawnPoints)
	{
		if (IsValid(SpawnPoint)) SpawnPoint->Destroy();
	}
	SpawnPoints.Empty();
	
	if (EncounterSpawnData)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		
		FVector Offset = FVector{ 100.f, 0.f, 0.f};
		for (FEncounterSpawn Spawn : EncounterSpawnData->EncounterSpawns)
		{
			AEncounterSpawnPoint* SpawnPoint = GetWorld()->SpawnActor<AEncounterSpawnPoint>(AEncounterSpawnPoint::StaticClass(), GetActorLocation() + Offset, FRotator::ZeroRotator, SpawnParams);
			if(Spawn.OverrideBehaviourData) Spawn.EncounterData->BehaviourData = Spawn.OverrideBehaviourData;
				
			SpawnPoint->EncounterClass = Spawn.EncounterClass;
			SpawnPoint->EncounterLevel = Spawn.Level;
			SpawnPoint->EncounterData = Spawn.EncounterData;
			SpawnPoint->RespawnTime = Spawn.RespawnTime;
			SpawnPoint->SpawnerType = Spawn.SpawnerType;
			SpawnPoint->SpawnLocationType = Spawn.SpawnLocationType;
			SpawnPoint->PreferredSpawningRange = Spawn.PreferredSpawningRange;
			SpawnPoint->PointCollectionClass = Spawn.PointCollectionClass;
			SpawnPoint->Count = Spawn.Count;
			SpawnPoints.Add(SpawnPoint);
			Offset.X += 50.f;
		}
	}
}

void AEncounterSpawnVolume::UpdateSpawnPoints()
{	
}

void AEncounterSpawnVolume::BeginPlay()
{
	Super::BeginPlay();
	if (HasAuthority())
	{
		TriggerVolume->OnComponentBeginOverlap.AddDynamic(this, &AEncounterSpawnVolume::OnBeginOverlap);
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
				SpawnPoint->SpawnEncounter();
			}
		}
		if (SpawnerType == ESpawnerType::Once) bActivated = true;
		DisableVolume();
	}
}

void AEncounterSpawnVolume::HandlePlayerLeaving()
{
	EnableVolume();
	for (AEncounterSpawnPoint* SpawnPoint : SpawnPoints)
	{
		if (SpawnPoint) SpawnPoint->DespawnEncounter();
	}
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