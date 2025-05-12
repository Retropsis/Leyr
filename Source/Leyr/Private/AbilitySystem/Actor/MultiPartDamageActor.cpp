// @ Retropsis 2024-2025.

#include "AbilitySystem/Actor/MultiPartDamageActor.h"

AMultiPartDamageActor::AMultiPartDamageActor()
{
	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(Root);
	
	MultiPartFlipbook = CreateDefaultSubobject<USkeletalMeshComponent>("MultiPartFlipbook");
	MultiPartFlipbook->SetupAttachment(GetRootComponent());
	MultiPartFlipbook->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MultiPartFlipbook->SetCollisionResponseToAllChannels(ECR_Ignore);
}

void AMultiPartDamageActor::InteractHit_Implementation(AActor* InteractingActor, FName BoneName)
{
	if(bCanBreak && BoneName != UnbreakableRoot)
	{
		MultiPartFlipbook->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
		MultiPartFlipbook->SetAllBodiesBelowSimulatePhysics(BoneName, true);
		MultiPartFlipbook->AddImpulseToAllBodiesBelow(InteractingActor->GetActorForwardVector() * ImpactImpulse, BoneName, true);
		HandleDetachment(BoneName);
	}
}

void AMultiPartDamageActor::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor->ActorHasTag("Player"))
	{
		HandleBeginOverlap();
		CauseDamage(OtherActor);
	}
}
