// @ Retropsis 2024-2025.

#include "World/Level/Platform.h"
#include "Components/BoxComponent.h"
#include "Leyr/Leyr.h"

APlatform::APlatform()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>("BoxCollision");
	BoxCollision->SetCollisionObjectType(ECC_OneWayPlatform);
	BoxCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
	BoxCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	BoxCollision->SetCollisionResponseToChannel(ECC_Player, ECR_Overlap);
	SetRootComponent(BoxCollision);

	Tags.Add(FName("Platform"));
}

void APlatform::BeginPlay()
{
	Super::BeginPlay();
	SetActorTickEnabled(false);
}

void APlatform::ToggleActivate_Implementation(bool bActivate)
{
	SetActorTickEnabled(bActivate);
}
