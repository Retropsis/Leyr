// @ Retropsis 2024-2025.

#include "World/Level/Moving/WaterGroup.h"

#include "Components/BoxComponent.h"
#include "Interaction/PlayerInterface.h"
#include "Kismet/KismetSystemLibrary.h"

AWaterGroup::AWaterGroup()
{
}

void AWaterGroup::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	OverlapBox->SetBoxExtent(FVector(Width * 32.f, 100.f, Depth * 32.f));
	SurfaceZ = OverlapBox->GetComponentLocation().Z + OverlapBox->GetScaledBoxExtent().Z;
}

void AWaterGroup::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor && OtherActor->Implements<UPlayerInterface>())
	{
		ActiveActors.AddUnique(OtherActor);
		IPlayerInterface::Execute_HandleSwimming(OtherActor, MinZ, SwimmingSpeed, SwimmingGravityScale, false);
	}
}

void AWaterGroup::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(OtherActor && OtherActor->Implements<UPlayerInterface>() && ActiveActors.Contains(OtherActor))
	{
		ActiveActors.Remove(OtherActor);
		IPlayerInterface::Execute_HandleSwimming(OtherActor, 0.f, 0.f, 0.f, true);
	}
}

void AWaterGroup::HandleActiveActors(float DeltaSeconds)
{
	if(ActiveActors.Num() == 0) return;

	for (AActor* Actor : ActiveActors)
	{
		if(Actor)
		{
			const bool bSinkFaster = Actor->GetActorLocation().Z > SurfaceZ;
			
			FVector CurrentBottom = FVector(Actor->GetActorLocation().X, 0.f, InterpTarget.Z);
			UKismetSystemLibrary::DrawDebugPoint(this, InterpTarget, 15.f, FLinearColor::Green);
			switch (MovingDirection) {
			case EMovingDirection::Sink:
				Actor->SetActorLocation(FMath::VInterpTo(Actor->GetActorLocation(), CurrentBottom, DeltaSeconds, 10.f));
				// Actor->SetActorLocation(FMath::VInterpTo(Actor->GetActorLocation(), CurrentBottom, DeltaSeconds, bSinkFaster ? 10.f : InterpolationSpeed));
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
