// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "World/Level/MovingPlatform.h"
#include "Trolley.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTrolleyFalling);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTrolleyRespawn);

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

	UPROPERTY(BlueprintAssignable) FOnTrolleyFalling OnTrolleyFalling;
	UPROPERTY(BlueprintAssignable) FOnTrolleyRespawn OnTrolleyRespawn;
	
protected:
	virtual void BeginPlay() override;

	void HandleOverlapWaitTimeEnd();
	void HandleRespawnTimeEnd();
	void HandleFallingTimeEnd();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Platform|Trolley")
	bool bIsSingleUse = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Platform|Trolley")
	float OverlapWaitTime = .5f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Platform|Trolley")
	float RespawnTime = 10.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Platform|Trolley")
	float FallingTime = 1.5f;
	
private:
	FTimerHandle OverlapWaitTimer;
	FTimerHandle RespawnTimer;
	FTimerHandle FallingTimer;
};
