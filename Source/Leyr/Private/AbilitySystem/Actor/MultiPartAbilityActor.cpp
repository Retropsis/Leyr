// @ Retropsis 2024-2025.

#include "AbilitySystem/Actor/MultiPartAbilityActor.h"

AMultiPartAbilityActor::AMultiPartAbilityActor()
{
	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(Root);
	
	MultiPartFlipbook = CreateDefaultSubobject<USkeletalMeshComponent>("MultiPartFlipbook");
	MultiPartFlipbook->SetupAttachment(GetRootComponent());
	MultiPartFlipbook->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MultiPartFlipbook->SetCollisionResponseToAllChannels(ECR_Ignore);
}

void AMultiPartAbilityActor::InteractHit_Implementation(AActor* InteractingActor, FName BoneName)
{
	if(bCanBreak && BoneName != UnbreakableRoot)
	{
		// MultiPartFlipbook->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		MultiPartFlipbook->SetAllBodiesBelowSimulatePhysics(BoneName, true);
		MultiPartFlipbook->AddImpulseToAllBodiesBelow(InteractingActor->GetActorForwardVector() * ImpactImpulse, BoneName, true);
		HandleDetachment(BoneName);
	}
}

void AMultiPartAbilityActor::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor->ActorHasTag("Player"))
	{
		MultiPartFlipbook->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
		MultiPartFlipbook->SetAllBodiesBelowSimulatePhysics(FName("First"), true);
		MultiPartFlipbook->AddImpulseToAllBodiesBelow(OtherActor->GetActorForwardVector() * ImpactImpulse, FName("Tail"), true);
	}
}
