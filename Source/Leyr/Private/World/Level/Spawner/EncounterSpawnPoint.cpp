// @ Retropsis 2024-2025.

#include "World/Level/Spawner/EncounterSpawnPoint.h"
#include "AI/AICharacter.h"

void AEncounterSpawnPoint::SpawnEncounter()
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	AAICharacter* Encounter = GetWorld()->SpawnActorDeferred<AAICharacter>(EncounterClass, GetActorTransform());
	Encounter->SetEncounterLevel(EncounterLevel);
	Encounter->SetEncounterClass(CharacterClass);
	Encounter->FinishSpawning(GetActorTransform());
	Encounter->SpawnDefaultController();
}
