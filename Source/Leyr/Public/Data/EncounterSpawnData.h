// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "EncounterSpawnData.generated.h"

class UBehaviourData;
class UEncounterData;
/**
 * 
 */
UCLASS(BlueprintType)
class LEYR_API UEncounterSpawnData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	int32 Level = 1;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UEncounterData> EncounterData = nullptr;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UBehaviourData> OverrideBehaviourData = nullptr;
};
