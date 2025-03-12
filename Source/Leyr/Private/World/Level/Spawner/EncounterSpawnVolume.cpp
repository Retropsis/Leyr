// @ Retropsis 2024-2025.

#include "World/Level/Spawner/EncounterSpawnVolume.h"
#include "Components/BoxComponent.h"
#include "Interaction/PlayerInterface.h"
#include "Leyr/Leyr.h"
#include "World/Level/Spawner/EncounterSpawnPoint.h"

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

void AEncounterSpawnVolume::BeginPlay()
{
	Super::BeginPlay();
	if (HasAuthority())
	{
		BoxOverlap->OnComponentBeginOverlap.AddDynamic(this, &AEncounterSpawnVolume::OnBeginOverlap);
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
				SpawnPoint->SpawnEncounter();
			}
		}
		if (SpawnerType == ESpawnerType::Once)
		{
			bActivated = true;
			DisableVolume();
		}
	}
}

void AEncounterSpawnVolume::DisableVolume() const
{
	BoxOverlap->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BoxOverlap->SetCollisionResponseToAllChannels(ECR_Ignore);
}
