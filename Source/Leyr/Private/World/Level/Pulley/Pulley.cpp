// @ Retropsis 2024-2025.

#include "World/Level/Pulley/Pulley.h"
#include "Components/BoxComponent.h"

APulley::APulley()
{
	PrimaryActorTick.bCanEverTick = false;
	LeverType = ELeverType::SingleUse;

	WeightHitBox = CreateDefaultSubobject<UBoxComponent>("WeightHitBox");
	WeightHitBox->SetupAttachment(GetRootComponent());
	bShouldBlockProjectile = false;
}

void APulley::LoadActor_Implementation()
{
	Super::LoadActor_Implementation();
	WeightHitBox->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	WeightHitBox->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	WeightHitBox->SetEnableGravity(true);
	WeightHitBox->SetSimulatePhysics(true);
}

void APulley::BeginPlay()
{
	// Skip Lever BeginPlay
	APaperFlipbookActor::BeginPlay();
	if (HasAuthority())
	{
		OverlapBox->OnComponentBeginOverlap.AddDynamic(this, &ALever::OnBeginOverlap);
	}
}

void APulley::Interact_Implementation(AActor* InteractingActor)
{
	Super::Interact_Implementation(InteractingActor);
	WeightHitBox->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	WeightHitBox->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	WeightHitBox->SetEnableGravity(true);
	WeightHitBox->SetSimulatePhysics(true);
}

// void APulley::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
// {
// 	if(OtherActor&& OtherActor->ActorHasTag("HitInteraction"))
// 	{
// 		// Skip Lever OnBeginOverlap
// 	}
// }

