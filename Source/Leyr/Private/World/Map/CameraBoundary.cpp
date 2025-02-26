// @ Retropsis 2024-2025.

#include "World/Map/CameraBoundary.h"
#include "Components/BoxComponent.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Interaction/PlayerInterface.h"
#include "Kismet/KismetMathLibrary.h"

ACameraBoundary::ACameraBoundary()
{
	PrimaryActorTick.bCanEverTick = true;

	Boundary = CreateDefaultSubobject<UBoxComponent>("Boundary");
	Boundary->InitBoxExtent(FVector(50.f));
	SetRootComponent(Boundary);

	BoundaryVisualizer = CreateDefaultSubobject<UStaticMeshComponent>("BoundaryVisualizer");
	BoundaryVisualizer->SetupAttachment(GetRootComponent());
	BoundaryVisualizer->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BoundaryVisualizer->SetHiddenInGame(true);

	TimelineComponent = CreateDefaultSubobject<UTimelineComponent>("Timeline");
}

void ACameraBoundary::BeginPlay()
{
	Super::BeginPlay();
	SetActorTickEnabled(false);
	Boundary->OnComponentBeginOverlap.AddDynamic(this, &ACameraBoundary::OnBeginOverlap);
	Boundary->OnComponentEndOverlap.AddDynamic(this, &ACameraBoundary::OnEndOverlap);
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

void ACameraBoundary::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor && OtherActor->Implements<UPlayerInterface>())
	{
		TimelineComponent->Stop();
		ExitLocation = OtherActor->GetActorLocation();
		Player = OtherActor;
		SpringArm = IPlayerInterface::Execute_GetSpringArmComponent(OtherActor);
		if (SpringArm)
		{
			// FTimerHandle DetachTimer;
			FDetachmentTransformRules DetachmentTransformRules{
				EDetachmentRule::KeepWorld,
				EDetachmentRule::KeepRelative,
				EDetachmentRule::KeepRelative,
				false};
			SpringArm->DetachFromComponent(DetachmentTransformRules);
			// GetWorld()->GetTimerManager().SetTimer(DetachTimer, FTimerDelegate::CreateLambda([this]()
			// {
			// }), .25f, false);

			
			// SpringArm->bEnableCameraLag = false;
			// OriginalSocketOffset = SpringArm->SocketOffset;
			SetActorTickEnabled(true);
			bShouldInterpZ = true;
		}
	}
}

void ACameraBoundary::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(OtherActor && OtherActor->Implements<UPlayerInterface>())
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
			SpringArm->AttachToComponent(OtherActor->GetRootComponent(), AttachmentTransformRules);
			SpringArm->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
			SpringArm->bEnableCameraLag = true;
			// ResetSocketOffset = SpringArm->SocketOffset;
			// ResetSocketOffset = FVector(0.f);
			SetActorTickEnabled(false);
			// ResetSocket();
		}
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

