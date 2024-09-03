// @ Retropsis 2024-2025.

#include "World/Level/Rope.h"
#include "Components/BoxComponent.h"
#include "Interaction/PlayerInterface.h"

ARope::ARope()
{
	BuildDirection = EBuildDirection::Horizontal;
}

void ARope::BeginPlay()
{
	Super::BeginPlay();
	if(HasAuthority())
	{
		HangingCollision->OnComponentBeginOverlap.AddDynamic(this, &ARope::OnBeginOverlap);
		HangingCollision->OnComponentEndOverlap.AddDynamic(this, &ARope::OnEndOverlap);
	}
}

void ARope::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherComp && OtherComp->ComponentHasTag("RopeCollision"))
	{
		const FVector Location{0.f, 0.f, HangingCollision->GetComponentLocation().Z};
		IPlayerInterface::Execute_HandleHangingOnRope(OtherActor, Location, false);
	}
}

void ARope::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(OtherComp && OtherComp->ComponentHasTag("RopeCollision"))
	{
		HangingCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
		if (const UWorld* World = GetWorld()) World->GetTimerManager().SetTimer(IgnoreCollisionTimer, this, &ARope::HandleIgnoreCollisionEnd, IgnoreCollisionTime);
		
		IPlayerInterface::Execute_HandleHangingOnRope(OtherActor, FVector::ZeroVector, true);
	}
}

void ARope::HandleIgnoreCollisionEnd()
{
	HangingCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}
