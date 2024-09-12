// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "World/Level/MovingPlatform.h"
#include "Trolley.generated.h"

/**
 * 
 */
UCLASS()
class LEYR_API ATrolley : public AMovingPlatform
{
	GENERATED_BODY()

public:
	ATrolley();
	virtual void Tick(float DeltaSeconds) override;
	virtual void ChooseNextStep() override;
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

protected:
	virtual void BeginPlay() override;

	UFUNCTION() void HandleOverlapWaitTimeEnd();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Platform|Trolley")
	float OverlapWaitTime = .5f;
	
private:
	FTimerHandle OverlapWaitTimer;
};
