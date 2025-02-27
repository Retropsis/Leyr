// @ Retropsis 2024-2025.

#include "World/Level/Zone/Arena.h"
#include "Components/BoxComponent.h"
#include "Interaction/PlayerInterface.h"
#include "Leyr/Leyr.h"

AArena::AArena()
{
	PrimaryActorTick.bCanEverTick = false;

	LeavingArea = CreateDefaultSubobject<UBoxComponent>("LeavingArea");
	LeavingArea->SetupAttachment(GetRootComponent());
	LeavingArea->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	LeavingArea->SetCollisionResponseToAllChannels(ECR_Ignore);
	LeavingArea->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	LeavingArea->SetCollisionResponseToChannel(ECC_Player, ECR_Overlap);
	LeavingArea->InitBoxExtent(FVector(360.f, 100.f, 360.f));

	LeavingBoundaryVisualizer = CreateDefaultSubobject<UStaticMeshComponent>("LeavingBoundaryVisualizer");
	LeavingBoundaryVisualizer->SetupAttachment(GetRootComponent());
	LeavingBoundaryVisualizer->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	LeavingBoundaryVisualizer->SetHiddenInGame(true);

	BoundaryRule = EBoundaryRule::Arena;
}

void AArena::BeginPlay()
{
	Super::BeginPlay();
	SetActorTickEnabled(false);
	if (HasAuthority())
	{
		Boundary->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		
		LeavingArea->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		LeavingArea->OnComponentEndOverlap.AddDynamic(this, &AArena::OnEndOverlap);
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
		LeavingArea->SetBoxExtent(Boundary->GetScaledBoxExtent() + 40.f);
		LeavingBoundaryVisualizer->SetWorldScale3D(FVector{ Boundary->GetScaledBoxExtent().X / 50.f, 2.f, Boundary->GetScaledBoxExtent().Z / 50.f });
	}
}

void AArena::HandleOnBeginOverlap(AActor* OtherActor)
{
	Super::HandleOnBeginOverlap(OtherActor);
	OnPlayerEntering.Broadcast(OtherActor);
}

FBoundLocations AArena::GetArenaBounds() const
{
	const float ArenaExtent = Boundary->GetScaledBoxExtent().X;
	const FVector Location = Boundary->GetComponentLocation();
	return FBoundLocations{ FVector{ Location.X - ArenaExtent, 0.f, Location.Z }, FVector{ Location.X + ArenaExtent, 0.f, Location.Z } };
}
