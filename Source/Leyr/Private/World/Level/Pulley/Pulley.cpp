// @ Retropsis 2024-2025.

#include "World/Level/Pulley/Pulley.h"
#include "Components/BoxComponent.h"
#include "Leyr/Leyr.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"

APulley::APulley()
{
	PrimaryActorTick.bCanEverTick = false;
	LeverType = ELeverType::SingleUse;

	WeightHitBox = CreateDefaultSubobject<UBoxComponent>("WeightHitBox");
	WeightHitBox->SetupAttachment(GetRootComponent());
	WeightHitBox->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	WeightHitBox->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	WeightHitBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
	WeightHitBox->SetCollisionResponseToChannel(ECC_Player, ECR_Block);
	WeightHitBox->SetCollisionResponseToChannel(ECC_Projectile, ECR_Block);
	WeightHitBox->SetEnableGravity(true);
	WeightHitBox->SetSimulatePhysics(true);

	PhysicsConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>("PhysicsConstraint");
	PhysicsConstraint->SetupAttachment(GetRootComponent());
	PhysicsConstraint->ComponentName1.ComponentName = OverlapBox->GetFName();
	PhysicsConstraint->ComponentName2.ComponentName = WeightHitBox->GetFName();
	
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

void APulley::InteractHit_Implementation(AActor* InteractingActor, FName BoneName)
{
	Super::InteractHit_Implementation(InteractingActor, BoneName);
	
	LeverState = ELeverState::On;
	HandleLeverVisualState(LeverState);
	OnLeverStateChanged.Broadcast(LeverState);
	
	if(PhysicsConstraint)
	{
		PhysicsConstraint->BreakConstraint();
		WeightHitBox->SetSimulatePhysics(true);
		WeightHitBox->OnComponentHit.AddDynamic(this, &APulley::OnHit);
		WeightHitBox->SetCollisionResponseToAllChannels(ECR_Ignore);
		WeightHitBox->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	}
}

void APulley::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	WeightHitBox->OnComponentHit.Clear();
	// WeightHitBox->SetSimulatePhysics(false);
	// WeightHitBox->SetEnableGravity(false);
}
