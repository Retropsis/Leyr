// @ Retropsis 2024-2025.

#include "World/Map/CameraBoundary.h"
#include "Components/BoxComponent.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Interaction/PlayerInterface.h"
#include "PaperTileMapComponent.h"
#include "Kismet/KismetMathLibrary.h"

ACameraBoundary::ACameraBoundary()
{
	PrimaryActorTick.bCanEverTick = true;

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

	TimelineComponent = CreateDefaultSubobject<UTimelineComponent>("Timeline");
}

void ACameraBoundary::BeginPlay()
{
	Super::BeginPlay();
	SetActorTickEnabled(false);
	EnteringBoundary->OnComponentBeginOverlap.AddDynamic(this, &ACameraBoundary::OnBeginOverlap);
	// EnteringBoundary->OnComponentEndOverlap.AddDynamic(this, &ACameraBoundary::OnEndOverlap);
}

void ACameraBoundary::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	SetSocket();
	
	if (bShouldInterpZ)
	{
		Alpha+= DeltaTime * .5f;
		const FVector Start{ SpringArm->GetComponentLocation().X, 0.f, SpringArm->GetComponentLocation().Z };
		const FVector Target{ SpringArm->GetComponentLocation().X, 0.f, GetActorLocation().Z };
		SpringArm->SetWorldLocation(UKismetMathLibrary::VLerp(Start, Target, Alpha));
		if(Alpha >= 1.f) bShouldInterpZ = false;
	}
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
		HandleOnEndOverlap(OtherActor);
	}
}

void ACameraBoundary::SetSocket()
{
	if(SpringArm == nullptr) return;
	
	switch (ExitDirection) {
	case EExitDirection::Vertical:
		SpringArm->SocketOffset = FVector(SpringArm->SocketOffset.X, SpringArm->SocketOffset.Y, VerticalSocketOffsetZ());
		break;
	case EExitDirection::Horizontal:
		// SpringArm->SocketOffset = FVector(SpringArm->SocketOffset.X, HorizontalSocketOffsetY(), SpringArm->SocketOffset.Z);
		// FollowCamera->ClearAdditiveOffset();
		// FollowCamera->AddAdditiveOffset(FTransform{ FVector(GetDistanceTo(Player), 0.f, 0.f) }, 90.f);
		break;
	}
}

void ACameraBoundary::HandleOnBeginOverlap(AActor* OtherActor)
{
	TimelineComponent->Stop();
	ExitLocation = OtherActor->GetActorLocation();
	Player = OtherActor;
	SpringArm = IPlayerInterface::Execute_GetSpringArmComponent(OtherActor);
	if (SpringArm)
	{
		FDetachmentTransformRules DetachmentTransformRules{
			EDetachmentRule::KeepWorld,
			EDetachmentRule::KeepRelative,
			EDetachmentRule::KeepRelative,
			false};
		switch (BoundaryRule) {
		case EBoundaryRule::Detachment:
			SpringArm->DetachFromComponent(DetachmentTransformRules);
			SetActorTickEnabled(true);
			bShouldInterpZ = true;
			break;
		case EBoundaryRule::Extent:
			IPlayerInterface::Execute_SetCameraInterpolation(OtherActor, this, ECameraInterpState::Entering);
			break;
		case EBoundaryRule::Arena:
			IPlayerInterface::Execute_SetCameraInterpolation(OtherActor, this, ECameraInterpState::Entering);
			break;
		}
	}
}

void ACameraBoundary::HandleOnEndOverlap(AActor* OtherActor)
{
	Player = OtherActor;
	SpringArm = IPlayerInterface::Execute_GetSpringArmComponent(OtherActor);
	if (SpringArm)
	{
		FAttachmentTransformRules AttachmentTransformRules{
			EAttachmentRule::SnapToTarget,
			EAttachmentRule::SnapToTarget,
			EAttachmentRule::KeepRelative,
			false};
		switch (BoundaryRule) {
		case EBoundaryRule::Detachment:
			SpringArm->AttachToComponent(OtherActor->GetRootComponent(), AttachmentTransformRules);
			SpringArm->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
			SpringArm->bEnableCameraLag = true;
			SetActorTickEnabled(false);
			break;
		case EBoundaryRule::Extent:
			IPlayerInterface::Execute_SetCameraInterpolation(OtherActor, this, ECameraInterpState::Leaving);
			break;
		case EBoundaryRule::Arena:
			IPlayerInterface::Execute_SetCameraInterpolation(OtherActor, this, ECameraInterpState::Leaving);
			break;
		}
	}
}

float ACameraBoundary::VerticalSocketOffsetZ() const
{
	return ExitLocation.Z - Player->GetActorLocation().Z + OriginalSocketOffset.Z;
}

float ACameraBoundary::HorizontalSocketOffsetY() const
{
	return ExitLocation.X + OriginalSocketOffset.X - Player->GetActorLocation().X + OriginalSocketOffset.Y;
}

void ACameraBoundary::InterpSocketOffset(float InAlpha)
{
	FVector SocketOffset = SpringArm->SocketOffset;
	switch (ExitDirection) {
	case EExitDirection::Vertical:
		SocketOffset = FVector(SocketOffset.X, SocketOffset.Y, OriginalSocketOffset.Z);
		break;
	case EExitDirection::Horizontal:
		SocketOffset = FVector(SocketOffset.X, OriginalSocketOffset.Y, SocketOffset.Z);
		break;
	}
	SpringArm->SocketOffset = UKismetMathLibrary::VLerp(ResetSocketOffset, SocketOffset, InAlpha);
}

