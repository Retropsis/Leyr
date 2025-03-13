// @ Retropsis 2024-2025.

#include "World/Level/Spawner/EncounterSpawnVolume.h"
#include "Components/BoxComponent.h"
#include "Data/EncounterData.h"
#include "Data/EncounterSpawnData.h"
#include "Interaction/PlayerInterface.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Leyr/Leyr.h"
#include "World/Level/Spawner/EncounterSpawnPoint.h"
#include "World/Map/CameraBoundary.h"

AEncounterSpawnVolume::AEncounterSpawnVolume()
{
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(Root);
	
	BoxOverlap = CreateDefaultSubobject<UBoxComponent>("BoxOverlap");
	BoxOverlap->SetupAttachment(GetRootComponent());
	BoxOverlap->InitBoxExtent(FVector{ 100.f });
	BoxOverlap->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BoxOverlap->SetCollisionResponseToAllChannels(ECR_Ignore);
	BoxOverlap->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	BoxOverlap->SetCollisionResponseToChannel(ECC_Player, ECR_Overlap);
	
	BoxVisualizer = CreateDefaultSubobject<UStaticMeshComponent>("BoxVisualizer");
	BoxVisualizer->SetupAttachment(BoxOverlap);
	BoxVisualizer->SetWorldScale3D(FVector{ 2.f });
	BoxVisualizer->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BoxVisualizer->SetHiddenInGame(true);
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
		
		bRandomizeLocation = EncounterSpawnData->bRandomizeLocation;
		SpawnerType = EncounterSpawnData->SpawnerType;

		FVector Offset = FVector::ZeroVector;
		for (FEncounterSpawn Spawn : EncounterSpawnData->EncounterSpawns)
		{
			AEncounterSpawnPoint* SpawnPoint = GetWorld()->SpawnActor<AEncounterSpawnPoint>(AEncounterSpawnPoint::StaticClass(), GetActorLocation() + Offset, FRotator::ZeroRotator, SpawnParams);
			if(Spawn.OverrideBehaviourData) Spawn.EncounterData->BehaviourData = Spawn.OverrideBehaviourData;
			SpawnPoint->EncounterClass = Spawn.EncounterClass;
			SpawnPoint->EncounterLevel = Spawn.Level;
			SpawnPoint->EncounterData = Spawn.EncounterData;
			SpawnPoint->RespawnTime = Spawn.RespawnTime;
			SpawnPoints.Add(SpawnPoint);
			Offset.X += 50.f;
		}
	}
}

void AEncounterSpawnVolume::BeginPlay()
{
	Super::BeginPlay();
	if (HasAuthority())
	{
		BoxOverlap->OnComponentBeginOverlap.AddDynamic(this, &AEncounterSpawnVolume::OnBeginOverlap);
		BindOnPlayerLeaving();
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
				SpawnPoint->bRandomizeLocation = bRandomizeLocation;
				SpawnPoint->SpawnerType = SpawnerType;

				if(EncounterSpawnData->bRandomizeLocation)
				{
					SpawnPoint->LeftBound = FVector{ GetActorLocation().X - BoxOverlap->GetScaledBoxExtent().X, 0.f, GetActorLocation().Z };
					SpawnPoint->RightBound = FVector{ GetActorLocation().X + BoxOverlap->GetScaledBoxExtent().X, 0.f, GetActorLocation().Z };
				}
				SpawnPoint->SpawnEncounter();
			}
		}
		if (SpawnerType == ESpawnerType::Once) bActivated = true;
		DisableVolume();
	}
}

void AEncounterSpawnVolume::BindOnPlayerLeaving()
{
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery2);
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	TArray<AActor*> OutActors;
	UKismetSystemLibrary::BoxOverlapActors(this, GetActorLocation(), FVector{ 50.f }, ObjectTypes, ACameraBoundary::StaticClass(), ActorsToIgnore, OutActors);
	for (AActor* Overlap : OutActors)
	{
		if (ACameraBoundary* CameraBoundary = Cast<ACameraBoundary>(Overlap))
		{
			CameraBoundary->OnPlayerLeaving.AddUObject(this, &AEncounterSpawnVolume::HandlePlayerLeaving);
			return;
		}
	}
}

void AEncounterSpawnVolume::HandlePlayerLeaving()
{
	for (AEncounterSpawnPoint* SpawnPoint : SpawnPoints)
	{
		if (SpawnPoint) SpawnPoint->DespawnEncounter();
		EnableVolume();
	}
}

void AEncounterSpawnVolume::DisableVolume() const
{
	BoxOverlap->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BoxOverlap->SetCollisionResponseToAllChannels(ECR_Ignore);
}

void AEncounterSpawnVolume::EnableVolume() const
{
	BoxOverlap->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BoxOverlap->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	BoxOverlap->SetCollisionResponseToChannel(ECC_Player, ECR_Overlap);
}
