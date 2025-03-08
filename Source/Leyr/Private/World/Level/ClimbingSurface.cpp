// @ Retropsis 2024-2025.

#include "World/Level/ClimbingSurface.h"
#include "Components/BoxComponent.h"
#include "Interaction/CombatInterface.h"
#include "Interaction/PlayerInterface.h"

AClimbingSurface::AClimbingSurface() {}

void AClimbingSurface::BeginPlay()
{
	Super::BeginPlay();
	if(HasAuthority())
	{
		HangingCollision->OnComponentBeginOverlap.AddDynamic(this, &AClimbingSurface::OnBeginOverlap);
		HangingCollision->OnComponentEndOverlap.AddDynamic(this, &AClimbingSurface::OnEndOverlap);
	}
}

void AClimbingSurface::Interact_Implementation(AActor* InteractingActor)
{
	ICombatInterface::Execute_SetCombatStateToHandle(InteractingActor, ECombatState::Climbing);
}

void AClimbingSurface::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	if(OtherActor && OtherActor->Implements<UPlayerInterface>())
	{
		const FVector Location{HangingCollision->GetComponentLocation().X, 0.f, 0.f};
		IPlayerInterface::Execute_HandleClimbing(OtherActor, Location, false);
	}
}

void AClimbingSurface::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
	if(OtherActor && OtherActor->Implements<UPlayerInterface>())
	{
		HangingCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
		if (const UWorld* World = GetWorld()) World->GetTimerManager().SetTimer(IgnoreCollisionTimer, this, &AClimbingSurface::HandleIgnoreCollisionEnd, IgnoreCollisionTime);
		IPlayerInterface::Execute_HandleClimbing(OtherActor, FVector::ZeroVector, true);
	}
}

void AClimbingSurface::HandleIgnoreCollisionEnd()
{
	HangingCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}
