// @ Retropsis 2024-2025.

#include "World/Level/Spawner/EncounterSpawnVolume.h"
#include "AI/AICharacter.h"
#include "Components/BillboardComponent.h"
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

void AEncounterSpawnVolume::LoadActor_Implementation()
{
	if (bActivated) DisableVolume();
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
			AEncounterSpawnPoint* SpawnPoint = GetWorld()->SpawnActor<AEncounterSpawnPoint>(AEncounterSpawnPoint::StaticClass(), GetActorLocation() + Offset, FRotator::ZeroRotator, SpawnParams);
			if(EncounterSpawnData.OverrideBehaviourData) EncounterSpawnData.EncounterData->BehaviourData = EncounterSpawnData.OverrideBehaviourData;
				
			SpawnPoint->EncounterClass = EncounterSpawnData.EncounterClass;
			SpawnPoint->EncounterLevel = EncounterSpawnData.Level;
			SpawnPoint->EncounterData = EncounterSpawnData.EncounterData;
			SpawnPoint->OverrideBehaviourData = EncounterSpawnData.OverrideBehaviourData;
			SpawnPoint->RespawnTime = EncounterSpawnData.RespawnTime;
			SpawnPoint->SpawnerType = EncounterSpawnData.SpawnerType;
			SpawnPoint->SpawnLocationType = EncounterSpawnData.SpawnLocationType;
			SpawnPoint->PreferredSpawningRange = EncounterSpawnData.PreferredSpawningRange;
			SpawnPoint->PointCollectionClass = EncounterSpawnData.PointCollectionClass;
			SpawnPoint->Count = bUniqueSpawnLocationType ? 1 : EncounterSpawnData.Count;
			if (EncounterSpawnData.EncounterData->EncounterIcon)
			{
				SpawnPoint->GetSpriteComponent()->SetSprite(EncounterSpawnData.EncounterData->EncounterIcon);
				UpdateEncounterIcon(EncounterSpawnData.EncounterData->EncounterIcon);
			}
			SpawnPoints.Add(SpawnPoint);
			Offset.X += 50.f;

			if (!bUniqueSpawnLocationType)
			{
				UE_LOG(LogTemp, Warning, TEXT("%s"), *UEnum::GetValueAsString(EncounterSpawnData.SpawnLocationType));
				return;
			}
		}
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
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	bool bUniqueSpawnLocationType = EncounterSpawnData.SpawnLocationType == ESpawnLocationType::Point || EncounterSpawnData.SpawnLocationType == ESpawnLocationType::AroundPoint;
	
	for (int i = 0; i < EncounterSpawnData.Count; ++i)
	{
		AEncounterSpawnPoint* SpawnPoint = SpawnPoints[i];

		if (SpawnPoint == nullptr) continue;
		
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
		if (EncounterSpawnData.EncounterData && EncounterSpawnData.EncounterData->EncounterIcon)
		{
			SpawnPoint->GetSpriteComponent()->SetSprite(EncounterSpawnData.EncounterData->EncounterIcon);
			UpdateEncounterIcon(EncounterSpawnData.EncounterData->EncounterIcon);
		}
		
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
		TriggerVolume->OnComponentBeginOverlap.AddDynamic(this, &AEncounterSpawnVolume::OnBeginOverlap);
		DespawningBounds->OnComponentEndOverlap.AddDynamic(this, &AEncounterSpawnVolume::OnDespawnOverlap);
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
			if (!ICombatInterface::Execute_IsDefeated(Encounter) && SpawnPoint->RequestRespawnEncounter(Encounter))
			{
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
	for (AEncounterSpawnPoint* SpawnPoint : SpawnPoints)
	{
		if (SpawnPoint) SpawnPoint->DespawnEncounter();
	}
}

void AEncounterSpawnVolume::HandlePlayerEntering()
{
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