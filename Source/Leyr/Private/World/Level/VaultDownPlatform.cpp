// @ Retropsis 2024-2025.

#include "World/Level/VaultDownPlatform.h"
#include "Components/BoxComponent.h"

AVaultDownPlatform::AVaultDownPlatform()
{
	OccupancyCollision = CreateDefaultSubobject<UBoxComponent>("OccupancyCollision");
	OccupancyCollision->SetupAttachment(GetRootComponent());
}

void AVaultDownPlatform::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	BoxCollision->SetBoxExtent(FVector(32.f * Length, 100.f, 16.f));
	OccupancyCollision->SetBoxExtent(FVector(32.f * Length, 100.f, 16.f));
}

void AVaultDownPlatform::SetBoxCollisionEnabled_Implementation(bool bEnabled)
{
	// BoxCollision->SetCollisionEnabled(bEnabled ? ECollisionEnabled::QueryOnly : ECollisionEnabled::NoCollision);
	// TODO: Probably change to ECC_PlayerCapsule
	BoxCollision->SetCollisionResponseToChannel(ECC_Pawn, bEnabled ? ECR_Block : ECR_Overlap);
}

void AVaultDownPlatform::BeginPlay()
{
	Super::BeginPlay();
	if(HasAuthority())
	{
		OccupancyCollision->OnComponentEndOverlap.AddDynamic(this, &AVaultDownPlatform::OnEndOverlap);
	}
}

void AVaultDownPlatform::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(OtherActor && OtherActor->ActorHasTag("Player"))
	{
		BoxCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	}
}
