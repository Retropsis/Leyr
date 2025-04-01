// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/BeamAbility.h"
#include "Electrocute.generated.h"

/**
 * 
 */
UCLASS()
class LEYR_API UElectrocute : public UBeamAbility
{
	GENERATED_BODY()

public:
	virtual FString GetDescription(int32 Level) override;
	virtual FString GetNextLevelDescription(int32 Level) override;
};
