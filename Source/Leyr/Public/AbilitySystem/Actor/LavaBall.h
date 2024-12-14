// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Actor/Spikes.h"
#include "LavaBall.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLand);

/**
 * 
 */
UCLASS()
class LEYR_API ALavaBall : public ASpikes
{
	GENERATED_BODY()

public:
	ALavaBall();
	virtual void Tick(float DeltaSeconds) override;
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent)
	void ShouldRotate(bool bRotate);

	UPROPERTY(BlueprintReadOnly)
	bool bLanded = false;

protected:
	void Launch();
	
	UPROPERTY(EditAnywhere, Category="AbilityActor|LavaBall")
	float LaunchCoolDown = 3.f;
	
	UPROPERTY(EditAnywhere, Category="AbilityActor|LavaBall")
	float LaunchImpulse = 750.f;

private:
	FTimerHandle LaunchTimer;
	float StoppingHeight = 0.f;
};
