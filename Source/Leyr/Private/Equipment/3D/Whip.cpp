// @ Retropsis 2024-2025.

#include "Equipment/3D/Whip.h"

AWhip::AWhip()
{
	PrimaryActorTick.bCanEverTick = false;

	Whip = CreateDefaultSubobject<USkeletalMeshComponent>("Whip");
	SetRootComponent(Whip);
	Whip->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Whip->SetCollisionResponseToAllChannels(ECR_Ignore);
}

