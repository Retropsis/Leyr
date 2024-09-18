// @ Retropsis 2024-2025.

#include "World/Level/Moving/MovingGroup.h"
#include "Components/BoxComponent.h"
#include "Interaction/PlayerInterface.h"
#include "Kismet/KismetSystemLibrary.h"

AMovingGroup::AMovingGroup()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	OverlapBox = CreateDefaultSubobject<UBoxComponent>("OverlapBox");
	SetRootComponent(OverlapBox);
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
	if(HasAuthority())
	{
		OverlapBox->OnComponentBeginOverlap.AddDynamic(this, &AMovingGroup::OnBeginOverlap);
		OverlapBox->OnComponentEndOverlap.AddDynamic(this, &AMovingGroup::OnEndOverlap);
	}
}

void AMovingGroup::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	OverlapBox->SetBoxExtent(FVector(Width * 32.f, 100.f, Depth * 32.f));
}

void AMovingGroup::HandleActiveActors(float DeltaSeconds)
{
	if(ActiveActors.Num() == 0) return;

	for (AActor* Actor : ActiveActors)
	{
		if(Actor)
		{
			FVector CurrentBottom = FVector(Actor->GetActorLocation().X, 0.f, InterpTarget.Z);
			UKismetSystemLibrary::DrawDebugPoint(this, InterpTarget, 15.f, FLinearColor::Green);
			switch (MovingDirection) {
			case EMovingDirection::Sink:
				Actor->SetActorLocation(FMath::VInterpTo(Actor->GetActorLocation(), CurrentBottom, DeltaSeconds, InterpolationSpeed));
				break;
			case EMovingDirection::Vector:
				UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + InterpTarget.GetSafeNormal(), 2.f, FLinearColor::Green);
				Actor->SetActorLocation(FMath::VInterpTo(Actor->GetActorLocation(), InterpTarget, DeltaSeconds, FlowingInterpSpeed));
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

