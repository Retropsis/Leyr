// @ Retropsis 2024-2025.

#include "World/Level/Spawner/EncounterSpawnPoint.h"
#include "Components/BillboardComponent.h"

AEncounterSpawnPoint::AEncounterSpawnPoint()
{
	PrimaryActorTick.bCanEverTick = false;
	GetSpriteComponent()->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f));
}

void AEncounterSpawnPoint::SetEncounterIcon(UTexture2D* Icon) const
{
	GetSpriteComponent()->SetSprite(Icon);
}
