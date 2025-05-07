// @ Retropsis 2024-2025.

#include "World/Level/PhysicsConstrainedActor.h"
#include "Components/BoxComponent.h"
#include "Leyr/Leyr.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"

APhysicsConstrainedActor::APhysicsConstrainedActor()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	
	HitBoxComponent = CreateDefaultSubobject<UBoxComponent>("HitBoxComponent");
	HitBoxComponent->SetupAttachment(GetRootComponent());
	HitBoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	HitBoxComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	HitBoxComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	HitBoxComponent->SetCollisionResponseToChannel(ECC_Player, ECR_Overlap);
	
	WeightBoxComponent = CreateDefaultSubobject<UBoxComponent>("WeightBoxComponent");
	WeightBoxComponent->SetupAttachment(GetRootComponent());
	WeightBoxComponent->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	WeightBoxComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	WeightBoxComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
	WeightBoxComponent->SetCollisionResponseToChannel(ECC_Player, ECR_Block);
	WeightBoxComponent->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	WeightBoxComponent->SetSimulatePhysics(true);
	WeightBoxComponent->SetEnableGravity(true);
	
	PhysicsConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>("PhysicsConstraintComponent");
	PhysicsConstraint->SetupAttachment(GetRootComponent());
	PhysicsConstraint->ComponentName1.ComponentName = HitBoxComponent->GetFName();
	PhysicsConstraint->ComponentName2.ComponentName = WeightBoxComponent->GetFName();
}

void APhysicsConstrainedActor::InteractHit_Implementation(AActor* InteractingActor, FName BoneName)
{
	if(bCanBreak && PhysicsConstraint)
	{
		PhysicsConstraint->BreakConstraint();
		WeightBoxComponent->SetSimulatePhysics(true);
	}
}

