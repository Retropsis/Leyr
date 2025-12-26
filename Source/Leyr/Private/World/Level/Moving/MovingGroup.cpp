// @ Retropsis 2024-2025.

#include "World/Level/Moving/MovingGroup.h"

#include "PaperSpriteComponent.h"
#include "Components/BoxComponent.h"
#include "Interaction/PlayerInterface.h"
#include "Kismet/KismetSystemLibrary.h"

AMovingGroup::AMovingGroup()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	OverlapBox = CreateDefaultSubobject<UBoxComponent>("OverlapBox");
	SetRootComponent(OverlapBox);

	Sprite = CreateDefaultSubobject<UPaperSpriteComponent>("WaterSprite");
	Sprite->SetupAttachment(OverlapBox);
	Sprite->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AMovingGroup::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	
	if (PropertyChangedEvent.Property->GetName() == TEXT("Width") || PropertyChangedEvent.Property->GetName() == TEXT("Depth"))
	{
		SetupWaterVolume(Width, Depth);
	}
}

void AMovingGroup::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	HandleActiveActors(DeltaSeconds);
}

void AMovingGroup::BeginPlay()
{
	Super::BeginPlay();
	MinZ = OverlapBox->GetComponentLocation().Z - OverlapBox->GetScaledBoxExtent().Z;
	InterpTarget = FVector(
		OverlapBox->GetComponentLocation().X + (OverlapBox->GetScaledBoxExtent().X - 35.f * Direction.X),
		0.f,
		OverlapBox->GetComponentLocation().Z - OverlapBox->GetScaledBoxExtent().Z + 88.f);
}

void AMovingGroup::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	OverlapBox->SetBoxExtent(FVector(Width * 32.f, 100.f, Depth * 32.f));
}

void AMovingGroup::SetupWaterVolumeFromBounds(const FBoxSphereBounds& Bounds)
{
	OverlapBox->SetBoxExtent(FVector(Bounds.BoxExtent.X, 100.f, Bounds.BoxExtent.Z));
	SurfaceZ = OverlapBox->GetComponentLocation().Z + OverlapBox->GetScaledBoxExtent().Z;
	Width = Bounds.BoxExtent.X / 32.f;
	Depth = Bounds.BoxExtent.Z / 32.f;
	Sprite->SetRelativeScale3D(FVector(Width, 1.f, Depth));
}

void AMovingGroup::SetupWaterVolume(const int32 NewWidth, const int32 NewDepth)
{
	Width = NewWidth;
	Depth = NewDepth;
	OverlapBox->SetBoxExtent(FVector(Width * 32.f, 100.f, Depth * 32.f));
	SurfaceZ = OverlapBox->GetComponentLocation().Z + OverlapBox->GetScaledBoxExtent().Z;
	Sprite->SetRelativeScale3D(FVector(Width, 1.f, Depth));
}

void AMovingGroup::ToggleActivate_Implementation(bool bActivate)
{
	if (!HasAuthority()) return;
	
	if (bActivate)
	{
		if (!OverlapBox->OnComponentBeginOverlap.IsAlreadyBound(this, &AMovingGroup::OnBeginOverlap)) OverlapBox->OnComponentBeginOverlap.AddDynamic(this, &AMovingGroup::OnBeginOverlap);
		if (!OverlapBox->OnComponentEndOverlap.IsAlreadyBound(this, &AMovingGroup::OnEndOverlap)) OverlapBox->OnComponentEndOverlap.AddDynamic(this, &AMovingGroup::OnEndOverlap);
	}
	else
	{
		ActiveActors.Empty();
		OverlapBox->OnComponentBeginOverlap.RemoveAll(this);
		OverlapBox->OnComponentEndOverlap.RemoveAll(this);
	}
}

void AMovingGroup::HandleActiveActors(float DeltaSeconds)
{
	if(ActiveActors.Num() == 0) return;

	for (TObjectPtr<AActor> Actor : ActiveActors)
	{
		if(IsValid(Actor))
		{
			FVector CurrentBottom = FVector(Actor->GetActorLocation().X, 0.f, InterpTarget.Z);
			UKismetSystemLibrary::DrawDebugPoint(this, InterpTarget, 15.f, FLinearColor::Green);
			switch (MovingDirection) {
			case EMovingDirection::Sink:
				Actor->SetActorLocation(FMath::VInterpConstantTo(Actor->GetActorLocation(), CurrentBottom, DeltaSeconds, InterpolationSpeed));
				IPlayerInterface::Execute_ReduceWalkSpeed(Actor, DeltaSeconds * EntangledInterpSpeed);
				break;
			case EMovingDirection::Vector:
				UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + InterpTarget.GetSafeNormal(), 2.f, FLinearColor::Green);
				Actor->SetActorLocation(FMath::VInterpConstantTo(Actor->GetActorLocation(), InterpTarget, DeltaSeconds, FlowingInterpSpeed));
				break;
			case EMovingDirection::Still:
				break;
			}
		}
	}
}

void AMovingGroup::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor && OtherActor->Implements<UPlayerInterface>())
	{
		ActiveActors.AddUnique(OtherActor);
		IPlayerInterface::Execute_HandleEntangled(OtherActor, MinZ, EntangledWalkSpeed, EntangledGravityScale, false);
	}
}

void AMovingGroup::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(OtherActor && OtherActor->Implements<UPlayerInterface>() && ActiveActors.Contains(OtherActor))
	{
		ActiveActors.Remove(OtherActor);
		IPlayerInterface::Execute_HandleEntangled(OtherActor, 0.f, 0.f, 0.f, true);
	}
}

FBoxSphereBounds AMovingGroup::GetNavigationBounds() const { return OverlapBox->Bounds; }