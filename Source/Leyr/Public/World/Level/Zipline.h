// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "World/Level/MovingPlatform.h"
#include "Zipline.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnZiplineFalling);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnZiplineRespawn);

/**
 * 
 */
UCLASS()
class LEYR_API AZipline : public AMovingPlatform
{
	GENERATED_BODY()

public:
	AZipline();
	virtual void Tick(float DeltaSeconds) override;
	virtual void ChooseNextStep() override;
	
	//~ LevelActorInterface
	virtual void ResetState_Implementation() override;
	//~ LevelActorInterface

	UPROPERTY(BlueprintAssignable) FOnZiplineFalling OnTrolleyFalling;
	UPROPERTY(BlueprintAssignable) FOnZiplineRespawn OnTrolleyRespawn;
	
protected:
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

	void HandleOverlapWaitTimeEnd();
	void HandleRespawnTimeEnd();
	void HandleFallingTimeEnd();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Platform|Trolley")
	bool bIsSingleUse = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Platform|Trolley")
	bool bShouldCollapse = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Platform|Trolley")
	float OverlapWaitTime = .1f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Platform|Trolley")
	float RespawnTime = 10.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Platform|Trolley")
	float FallingTime = 1.5f;
	
private:
	FTimerHandle OverlapWaitTimer;
	FTimerHandle RespawnTimer;
	FTimerHandle FallingTimer;
	bool bHasReachedTarget = false;
	bool bHasEndedOverlap = false;
};
