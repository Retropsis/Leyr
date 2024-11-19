// @ Retropsis 2024-2025.

#include "World/Level/Pulley/Pulley.h"
#include "Components/BoxComponent.h"

APulley::APulley()
{
	PrimaryActorTick.bCanEverTick = false;
	RopeHitBox = CreateDefaultSubobject<UBoxComponent>("RopeHitBox");
	RopeHitBox->SetupAttachment(GetRootComponent());

	WeightHitBox = CreateDefaultSubobject<UBoxComponent>("WeightHitBox");
	WeightHitBox->SetupAttachment(GetRootComponent());
}

void APulley::BeginPlay()
{
	Super::BeginPlay();
}

