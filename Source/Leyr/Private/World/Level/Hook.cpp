// @ Retropsis 2024-2025.

#include "World/Level/Hook.h"
#include "Components/BoxComponent.h"
#include "Interaction/PlayerInterface.h"

AHook::AHook() { }

void AHook::BeginPlay()
{
	Super::BeginPlay();
	if(HasAuthority())
	{
		HangingCollision->OnComponentBeginOverlap.AddDynamic(this, &AHook::OnBeginOverlap);
		HangingCollision->OnComponentEndOverlap.AddDynamic(this, &AHook::OnEndOverlap);
	}
}

void AHook::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	HangingCollision->SetBoxExtent(FVector(16.f * Length, 100.f, 16.f));
}

void AHook::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
  {
	if(OtherComp && OtherComp->ComponentHasTag("RopeCollision"))
	{
		const FVector Location{HangingCollision->GetComponentLocation().X, 0.f, HangingCollision->GetComponentLocation().Z};
		IPlayerInterface::Execute_HandleHangingOnHook(OtherActor, Location, false);
	}
}

void AHook::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(OtherComp && OtherComp->ComponentHasTag("RopeCollision"))
	{		
		IPlayerInterface::Execute_HandleHangingOnHook(OtherActor, FVector::ZeroVector, true);
	}
}

void AHook::HandleIgnoreCollisionEnd()
{
	HangingCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}
