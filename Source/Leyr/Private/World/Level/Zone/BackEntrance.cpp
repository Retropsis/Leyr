// @ Retropsis 2024-2025.

#include "World/Level/Zone/BackEntrance.h"
#include "Components/BoxComponent.h"

ABackEntrance::ABackEntrance()
{
	Tags.Add("BackEntrance");
	RequiredState = ECombatState::Unoccupied;
}

void ABackEntrance::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if(PairedBackEntrance == nullptr)
	{
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		PairedBackEntrance = GetWorld()->SpawnActor<ABackEntrance>(GetActorLocation(), FRotator::ZeroRotator, SpawnParameters);
		if(PairedBackEntrance)
		{
			PairedBackEntrance->PairedBackEntrance = this;
			PairedBackEntrance->OverlapZone->SetBoxExtent(OverlapZone->GetScaledBoxExtent());
		}
	}
}

bool ABackEntrance::TryBackEntrance_Implementation(AActor* InteractingActor, float CapsuleHalfHeight)
{
	FVector NewLocation = PairedBackEntrance->GetActorLocation();
	NewLocation.Z += OverlapZone->GetScaledBoxExtent().Z / 2.f - CapsuleHalfHeight;
	InteractingActor->SetActorLocation(NewLocation);
	return true;
}
