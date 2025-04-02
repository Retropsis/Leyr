// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/DamageGameplayAbility.h"
#include "FireBlast.generated.h"

class AFireBall;
/**
 * 
 */
UCLASS()
class LEYR_API UFireBlast : public UDamageGameplayAbility
{
	GENERATED_BODY()

public:
	virtual void InitAbility() override;
	virtual FString GetDescription(int32 Level) override;
	virtual FString GetNextLevelDescription(int32 Level) override;
	
	UFUNCTION(BlueprintCallable)
	TArray<AFireBall*> SpawnFireBalls();

	int32 MaxNumProjectiles = 10;
	
private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AFireBall> FireBallClass;
};
