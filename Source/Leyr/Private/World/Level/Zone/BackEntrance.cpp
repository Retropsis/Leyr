// @ Retropsis 2024-2025.

#include "World/Level/Zone/BackEntrance.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "Interaction/PlayerInterface.h"

ABackEntrance::ABackEntrance()
{
	Tags.Add("BackEntrance");
	RequiredState = ECombatState::Unoccupied;
	OverlapZone->SetCollisionObjectType(ECC_Interaction);
	OverlapZone->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	OverlapZone->SetCollisionResponseToChannel(ECC_Player, ECR_Overlap);
}

#if WITH_EDITOR
void ABackEntrance::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if(UWorld* World = GetWorld(); World && PairedBackEntrance == nullptr)
	{
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		const FVector PairedEntranceLocation{ GetActorLocation().X, 0.f, GetActorLocation().Z + 200.f };
		PairedBackEntrance = World->SpawnActor<ABackEntrance>(PairedEntranceLocation, FRotator::ZeroRotator, SpawnParameters);
		if(PairedBackEntrance)
		{
			PairedBackEntrance->PairedBackEntrance = this;
			PairedBackEntrance->OverlapZone->SetBoxExtent(OverlapZone->GetScaledBoxExtent());
		}
	}
}
#endif

void ABackEntrance::Interact_Implementation(AActor* InteractingActor)
{
	if(const ACharacter* Character = Cast<ACharacter>(InteractingActor))
	{
		FVector NewLocation = PairedBackEntrance->GetActorLocation();
        NewLocation.Z += OverlapZone->GetScaledBoxExtent().Z / 2.f - Character->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
        InteractingActor->SetActorLocation(NewLocation);
	}
	IPlayerInterface::Execute_DisableCameraLagForDuration(InteractingActor, 1.f);
}
