// @ Retropsis 2024-2025.

#include "World/Level/Zone/Arena.h"
#include "Components/BoxComponent.h"
#include "Leyr/Leyr.h"

AArena::AArena()
{
	PrimaryActorTick.bCanEverTick = false;

	EnteringArea = CreateDefaultSubobject<UBoxComponent>("EnteringArea");
	SetRootComponent(EnteringArea);
	EnteringArea->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	EnteringArea->SetCollisionResponseToAllChannels(ECR_Ignore);
	EnteringArea->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	EnteringArea->SetCollisionResponseToChannel(ECC_Player, ECR_Overlap);
	EnteringArea->InitBoxExtent(FVector(320.f, 100.f, 320.f));

	LeavingArea = CreateDefaultSubobject<UBoxComponent>("LeavingArea");
	LeavingArea->SetupAttachment(GetRootComponent());
	LeavingArea->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	LeavingArea->SetCollisionResponseToAllChannels(ECR_Ignore);
	LeavingArea->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	LeavingArea->SetCollisionResponseToChannel(ECC_Player, ECR_Overlap);
	LeavingArea->InitBoxExtent(FVector(360.f, 100.f, 360.f));

	EnteringBoundaryVisualizer = CreateDefaultSubobject<UStaticMeshComponent>("EnteringBoundaryVisualizer");
	EnteringBoundaryVisualizer->SetupAttachment(EnteringArea);
	EnteringBoundaryVisualizer->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	EnteringBoundaryVisualizer->SetHiddenInGame(true);

	LeavingBoundaryVisualizer = CreateDefaultSubobject<UStaticMeshComponent>("LeavingBoundaryVisualizer");
	LeavingBoundaryVisualizer->SetupAttachment(LeavingArea);
	LeavingBoundaryVisualizer->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	LeavingBoundaryVisualizer->SetHiddenInGame(true);
}

void AArena::BeginPlay()
{
	Super::BeginPlay();
	SetActorTickEnabled(false);
	if (HasAuthority())
	{
		EnteringArea->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		EnteringArea->OnComponentBeginOverlap.AddDynamic(this, &AArena::OnBeginOverlap);
		
		LeavingArea->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		LeavingArea->OnComponentEndOverlap.AddDynamic(this, &AArena::OnEndOverlap);
	}
}

void AArena::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor->ActorHasTag("Player")) OnPlayerEntering.Broadcast(OtherActor);
}

void AArena::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(OtherActor->ActorHasTag("Player")) OnPlayerLeaving.Broadcast(OtherActor);
}

FBoundLocations AArena::GetArenaBounds() const
{
	const float Extent = EnteringArea->GetScaledBoxExtent().X;
	const FVector Location = EnteringArea->GetComponentLocation();
	return FBoundLocations{ FVector{ Location.X - Extent, 0.f, Location.Z }, FVector{ Location.X + Extent, 0.f, Location.Z } };
}
