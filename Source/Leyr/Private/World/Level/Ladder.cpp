// @ Retropsis 2024-2025.

#include "World/Level/Ladder.h"
#include "Components/BoxComponent.h"
#include "Interaction/CombatInterface.h"
#include "Interaction/PlayerInterface.h"
#include "Leyr/Leyr.h"

ALadder::ALadder()
{
	BuildDirection = EBuildDirection::Vertical;
	HangingCollision->SetCollisionObjectType(ECC_Interaction);
	HangingCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	HangingCollision->SetCollisionResponseToChannel(ECC_Player, ECR_Overlap);
}

void ALadder::BeginPlay()
{
	Super::BeginPlay();
	if(HasAuthority())
	{
		HangingCollision->OnComponentBeginOverlap.AddDynamic(this, &ALadder::OnBeginOverlap);
		HangingCollision->OnComponentEndOverlap.AddDynamic(this, &ALadder::OnEndOverlap);
	}
}

void ALadder::Interact_Implementation(AActor* InteractingActor)
{
	ICombatInterface::Execute_SetCombatStateToHandle(InteractingActor, ECombatState::HangingLadder);
}

void ALadder::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor && OtherActor->Implements<UPlayerInterface>())
	{
		const FVector Location{HangingCollision->GetComponentLocation().X, 0.f, 0.f};
		IPlayerInterface::Execute_HandleHangingOnLadder(OtherActor, Location, false);
	}
}

void ALadder::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(OtherActor && OtherActor->Implements<UPlayerInterface>())
	{
		HangingCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
		if (const UWorld* World = GetWorld()) World->GetTimerManager().SetTimer(IgnoreCollisionTimer, this, &ALadder::HandleIgnoreCollisionEnd, IgnoreCollisionTime);
		IPlayerInterface::Execute_HandleHangingOnLadder(OtherActor, FVector::ZeroVector, true);
	}
}

void ALadder::HandleIgnoreCollisionEnd()
{
	HangingCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}
