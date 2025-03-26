// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/ProjectileAbility.h"
#include "BowAbility.generated.h"

/**
 * 
 */
UCLASS()
class LEYR_API UBowAbility : public UProjectileAbility
{
	GENERATED_BODY()

public:
	virtual void PrepareToEndAbility() override;
	virtual FString GetDescription(int32 Level) override;
	virtual FString GetNextLevelDescription(int32 Level) override;
};
