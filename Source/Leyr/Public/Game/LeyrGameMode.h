// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LeyrGameMode.generated.h"

class UMapInfo;
class UBehaviourInfo;
class UEncounterInfo;
class UActorClassInfo;
class UAbilityInfo;
class UItemAbilityInfo;
class UCharacterClassInfo;

/**
 * 
 */
UCLASS()
class LEYR_API ALeyrGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, Category = "Character Class Defaults")
	TObjectPtr<UCharacterClassInfo> CharacterClassInfo;
	
	UPROPERTY(EditDefaultsOnly, Category = "Actor Class Defaults")
	TObjectPtr<UActorClassInfo> ActorClassInfo;
	
	UPROPERTY(EditDefaultsOnly, Category = "Encounter Defaults")
	TObjectPtr<UEncounterInfo> EncounterInfo;
	
	UPROPERTY(EditDefaultsOnly, Category = "Ability Info")
	TObjectPtr<UAbilityInfo> AbilityInfo;
	
	UPROPERTY(EditDefaultsOnly, Category = "Ability Info")
	TObjectPtr<UItemAbilityInfo> ItemAbilityInfo;
	
	UPROPERTY(EditDefaultsOnly, Category = "Map Info")
	TObjectPtr<UMapInfo> MapInfo;
};
