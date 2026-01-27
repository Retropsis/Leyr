// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/SaveGame.h"
#include "World/Data/MapInfo.h"
#include "GameData.generated.h"

/**
 * 
 */
UCLASS()
class LEYR_API UGameData : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	TMap<FGameplayTag, FExplorationData> ExplorationData;
};
