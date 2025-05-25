// @ Retropsis 2024-2025.

#include "World/Map/EffectBoundary.h"
#include "Components/BoxComponent.h"
#include "Leyr/Leyr.h"

AEffectBoundary::AEffectBoundary()
{
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(Root);
	
	EnteringBoundary = CreateDefaultSubobject<UBoxComponent>("Entering Boundary");
	EnteringBoundary->InitBoxExtent(FVector(50.f));
	EnteringBoundary->SetupAttachment(GetRootComponent());
	EnteringBoundary->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	EnteringBoundary->SetCollisionResponseToAllChannels(ECR_Ignore);
	EnteringBoundary->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	EnteringBoundary->SetCollisionResponseToChannel(ECC_Player, ECR_Overlap);

	EnteringBoundaryVisualizer = CreateDefaultSubobject<UStaticMeshComponent>("Entering Boundary Visualizer");
	EnteringBoundaryVisualizer->SetupAttachment(EnteringBoundary);
	EnteringBoundaryVisualizer->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	EnteringBoundaryVisualizer->SetHiddenInGame(true);
	EnteringBoundaryVisualizer->bCastDynamicShadow = false;
}

