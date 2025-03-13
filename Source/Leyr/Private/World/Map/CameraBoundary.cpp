// @ Retropsis 2024-2025.

#include "World/Map/CameraBoundary.h"
#include "Components/BoxComponent.h"
#include "Interaction/PlayerInterface.h"
#include "PaperTileMapComponent.h"
#include "World/Data/CameraData.h"

ACameraBoundary::ACameraBoundary()
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
	
	NavigationBoundary = CreateDefaultSubobject<UBoxComponent>("Navigation Boundary");
	NavigationBoundary->InitBoxExtent(FVector(50.f));
	NavigationBoundary->SetupAttachment(GetRootComponent());
	NavigationBoundary->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	NavigationBoundary->SetCollisionResponseToAllChannels(ECR_Ignore);
	
	CameraBoundary = CreateDefaultSubobject<UBoxComponent>("Camera Extents");
	CameraBoundary->InitBoxExtent(FVector(50.f));
	CameraBoundary->SetupAttachment(GetRootComponent());
	CameraBoundary->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CameraBoundary->SetHiddenInGame(true);

	BoundaryVisualizer = CreateDefaultSubobject<UStaticMeshComponent>("Entering Boundary Visualizer");
	BoundaryVisualizer->SetupAttachment(GetRootComponent());
	BoundaryVisualizer->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BoundaryVisualizer->SetHiddenInGame(true);
}

void ACameraBoundary::BeginPlay()
{
	Super::BeginPlay();
	EnteringBoundary->OnComponentBeginOverlap.AddDynamic(this, &ACameraBoundary::OnBeginOverlap);
	EnteringBoundary->OnComponentEndOverlap.AddDynamic(this, &ACameraBoundary::OnEndOverlap);
}

void ACameraBoundary::InitializeCameraExtent()
{
	if (TileMap)
	{
		SetActorLocation(TileMap->GetRenderComponent()->Bounds.Origin);
		const FBoxSphereBounds Bounds = TileMap->GetRenderComponent()->Bounds;
		EnteringBoundary->SetBoxExtent(FVector{ Bounds.BoxExtent.X - 18.f, Bounds.BoxExtent.Y, Bounds.BoxExtent.Z - 54.f });
		NavigationBoundary->SetBoxExtent(FVector{ Bounds.BoxExtent.X - 128.f, Bounds.BoxExtent.Y, Bounds.BoxExtent.Z - 128.f });
		BoundaryVisualizer->SetWorldScale3D(FVector{ Bounds.BoxExtent.X / 50.f, Bounds.BoxExtent.Y / 50.f, Bounds.BoxExtent.Z / 50.f });
		CameraBoundary->SetBoxExtent(FVector{ bConstrainX ? FMath::Max(0.f, Bounds.BoxExtent.X - 640.f) : Bounds.BoxExtent.X, 0.f, bConstrainZ ? FMath::Max(0.f, Bounds.BoxExtent.Z - 384.f) : Bounds.BoxExtent.Z });
	}
}

void ACameraBoundary::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor && OtherActor->Implements<UPlayerInterface>())
	{
		HandleOnBeginOverlap(OtherActor);
	}
}

void ACameraBoundary::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(OtherActor && OtherActor->Implements<UPlayerInterface>())
	{
		OnPlayerLeaving.Broadcast();
	}
}

void ACameraBoundary::HandleOnBeginOverlap(AActor* OtherActor)
{
	if (OtherActor->Implements<UPlayerInterface>())
	{
		IPlayerInterface::Execute_SetCameraInterpolation(OtherActor, this, ECameraInterpState::Entering);
	}
}

