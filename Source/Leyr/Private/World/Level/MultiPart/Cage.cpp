// @ Retropsis 2024-2025.

#include "World/Level/MultiPart/Cage.h"

ACage::ACage()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ACage::InteractHit_Implementation(AActor* InteractingActor, FName BoneName)
{
	Super::InteractHit_Implementation(InteractingActor, BoneName);

	PlayFlashEffect(FlashStrength, 1 / FlashDuration, FlashColor);

	Health -= FMath::FRandRange(15.f, 24.f);
	if (Health <= 0.f)
	{
		DoorDestroyed();
	}
	else
	{
		PlayShakeEffect(ShakingStrength, 1 / ShakingDuration);
	}
}

