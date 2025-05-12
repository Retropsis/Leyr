// @ Retropsis 2024-2025.

#include "World/Level/MultiPart/MultiPartActor.h"
#include "Leyr/Leyr.h"

AMultiPartActor::AMultiPartActor()
{
	PrimaryActorTick.bCanEverTick = false;
	
	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(Root);
	
	MultiPartFlipbook = CreateDefaultSubobject<USkeletalMeshComponent>("MultiPartFlipbook");
	MultiPartFlipbook->SetupAttachment(GetRootComponent());
	MultiPartFlipbook->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	MultiPartFlipbook->SetCollisionResponseToAllChannels(ECR_Ignore);
	MultiPartFlipbook->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
	MultiPartFlipbook->SetCollisionResponseToChannel(ECC_Player, ECR_Block);
	MultiPartFlipbook->SetCollisionResponseToChannel(ECC_Enemy, ECR_Block);
}

void AMultiPartActor::InteractHit_Implementation(AActor* InteractingActor, FName BoneName)
{
	if(!UnbreakableBones.Contains(BoneName))
	{
		MultiPartFlipbook->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
		MultiPartFlipbook->SetAllBodiesBelowSimulatePhysics(FName("First"), true);
		MultiPartFlipbook->SetAllBodiesBelowSimulatePhysics(FName("First.R"), true);
		MultiPartFlipbook->SetCollisionResponseToAllChannels(ECR_Ignore);
		OnInteractHit(BoneName);
	}
}

void AMultiPartActor::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	MultiPartFlipbook->SetBodySimulatePhysics(SweepResult.BoneName, true);
}

void AMultiPartActor::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	MultiPartFlipbook->SetBodySimulatePhysics(OverlappedComponent->GetAttachSocketName(), false);
}