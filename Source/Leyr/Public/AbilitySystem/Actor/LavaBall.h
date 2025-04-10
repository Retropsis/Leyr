// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Actor/Spikes.h"
#include "LavaBall.generated.h"

UENUM(BlueprintType)
enum class ELavaBallState : uint8
{
	Idle,
	MovingUp,
	MovingDown,
};

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
	
	void StartLaunching();
	
	UFUNCTION(BlueprintImplementableEvent)
	void LaunchTimeline();

	UFUNCTION(BlueprintImplementableEvent)
	void HandleOnLand();

	UFUNCTION(BlueprintImplementableEvent)
	void ShouldRotate(bool bRotate);

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> ApexComponent;
	
	UPROPERTY(BlueprintReadOnly, Category="AbilityActor|LavaBall")
	FVector InitialLocation = FVector::ZeroVector;

	UPROPERTY(BlueprintReadOnly, Category="AbilityActor|LavaBall")
	FVector ApexLocation = FVector::ZeroVector;

	UPROPERTY(BlueprintReadOnly, Category="AbilityActor|LavaBall")
	float LaunchApex = 275.f;
	
	UPROPERTY(EditAnywhere, Category="AbilityActor|LavaBall")
	float LaunchCoolDown = 3.f;

	UPROPERTY(BlueprintReadWrite, Category="AbilityActor|LavaBall")
	ELavaBallState LavaBallState = ELavaBallState::Idle;

private:
	FTimerHandle LaunchTimer;
};
