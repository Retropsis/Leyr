// @ Retropsis 2024-2025.

#include "World/Level/Zone/Arena.h"
#include "AI/BaseCharacter.h"
#include "Components/BoxComponent.h"
#include "Leyr/Leyr.h"

AArena::AArena()
{
	PrimaryActorTick.bCanEverTick = false;

	LeavingBoundary = CreateDefaultSubobject<UBoxComponent>("Leaving Boundary");
	LeavingBoundary->SetupAttachment(GetRootComponent());
	LeavingBoundary->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	LeavingBoundary->SetCollisionResponseToAllChannels(ECR_Ignore);
	LeavingBoundary->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	LeavingBoundary->SetCollisionResponseToChannel(ECC_Player, ECR_Overlap);
	LeavingBoundary->InitBoxExtent(FVector(360.f, 100.f, 360.f));

	ArenaBoundary = CreateDefaultSubobject<UBoxComponent>("Arena Boundary");
	ArenaBoundary->SetupAttachment(GetRootComponent());
	ArenaBoundary->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ArenaBoundary->SetCollisionResponseToAllChannels(ECR_Ignore);
	ArenaBoundary->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	ArenaBoundary->SetCollisionResponseToChannel(ECC_Player, ECR_Overlap);
	ArenaBoundary->InitBoxExtent(FVector(360.f, 100.f, 360.f));

	LeavingBoundaryVisualizer = CreateDefaultSubobject<UStaticMeshComponent>("Leaving Boundary Visualizer");
	LeavingBoundaryVisualizer->SetupAttachment(GetRootComponent());
	LeavingBoundaryVisualizer->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	LeavingBoundaryVisualizer->SetHiddenInGame(true);
	LeavingBoundaryVisualizer->bCastDynamicShadow = false;

	BoundaryRule = EBoundaryRule::Arena;
}

void AArena::BeginPlay()
{
	Super::BeginPlay();
	SetActorTickEnabled(false);
	if (HasAuthority())
	{
		EnteringBoundary->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		
		LeavingBoundary->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		LeavingBoundary->OnComponentEndOverlap.AddDynamic(this, &AArena::OnLeavingBoundary);
	}
}

void AArena::OnLeavingBoundary(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	OnPlayerLeaving.Broadcast(OtherActor);
}

void AArena::InitializeCameraExtent()
{
	Super::InitializeCameraExtent();
	if (TileMap)
	{
		LeavingBoundary->SetBoxExtent(EnteringBoundary->GetScaledBoxExtent() + 40.f);
		LeavingBoundaryVisualizer->SetWorldScale3D(FVector{ LeavingBoundary->GetScaledBoxExtent().X / 50.f, LeavingBoundary->GetScaledBoxExtent().Y / 50.f, LeavingBoundary->GetScaledBoxExtent().Z / 50.f });
		ArenaBoundary->SetBoxExtent(FVector{ EnteringBoundary->GetScaledBoxExtent().X - 256.f, 125.f, EnteringBoundary->GetScaledBoxExtent().Z });
	}
}

void AArena::HandleOnBeginOverlap(AActor* OtherActor)
{
	Super::HandleOnBeginOverlap(OtherActor);
	OnPlayerEntering.Broadcast(OtherActor);
}

void AArena::HandleOnOwnerDeath(AActor* DeadActor)
{
	
}

FBoundLocations AArena::GetArenaBounds() const
{
	const float ArenaExtent = ArenaBoundary->GetScaledBoxExtent().X;
	const FVector Location = ArenaBoundary->GetComponentLocation();
	return FBoundLocations{ FVector{ Location.X - ArenaExtent, 0.f, Location.Z }, FVector{ Location.X + ArenaExtent, 0.f, Location.Z } };
}
