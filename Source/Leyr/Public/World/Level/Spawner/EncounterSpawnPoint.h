// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Engine/TargetPoint.h"
#include "EncounterSpawnPoint.generated.h"

class AAICharacter;
/**
 * 
 */
UCLASS()
class LEYR_API AEncounterSpawnPoint : public ATargetPoint
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SpawnEncounter();
	
	UPROPERTY(EditAnywhere, Category="Spawner")
	TSubclassOf<AAICharacter> EncounterClass;

	UPROPERTY(EditAnywhere, Category="Spawner")
	int32 EncounterLevel = 1;

	UPROPERTY(EditAnywhere, Category="Spawner")
	ECharacterClass CharacterClass = ECharacterClass::Warrior;
};
