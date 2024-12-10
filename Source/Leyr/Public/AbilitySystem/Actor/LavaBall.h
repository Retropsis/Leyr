// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Actor/Spikes.h"
#include "LavaBall.generated.h"

/**
 * 
 */
UCLASS()
class LEYR_API ALavaBall : public ASpikes
{
	GENERATED_BODY()

public:
	ALavaBall();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION(BlueprintImplementableEvent)
	void ShouldRotate(bool bRotate);

protected:
	void Launch();
	
	UPROPERTY(EditAnywhere, Category="AbilityActor|LavaBall")
	float LaunchCoolDown = 3.f;
	
	UPROPERTY(EditAnywhere, Category="AbilityActor|LavaBall")
	float LaunchImpulse = 750.f;

private:
	FTimerHandle LaunchTimer;
	bool bShouldRotate = false;
};
