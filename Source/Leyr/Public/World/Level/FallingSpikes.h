// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "WeightingPlatform.h"
#include "FallingSpikes.generated.h"

/**
 * 
 */
UCLASS()
class LEYR_API AFallingSpikes : public AActor
{
	GENERATED_BODY()

public:
	AFallingSpikes();
	virtual void Tick(float DeltaSeconds) override;
	
protected:
	virtual void BeginPlay() override;
	void MoveTo(const FVector& InCurrentTarget, float Speed, float InterpolationSpeed, float DeltaSeconds) const;
	bool HasReachedTarget() const;

	UFUNCTION() void HandleUpPositionTimeEnd();
	UFUNCTION() void HandleDownPositionTimeEnd();

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<USceneComponent> Anchor;
	
	UPROPERTY(EditAnywhere, Category="Spikes")
	float DownwardSpeed = 2300.f;
	
	UPROPERTY(EditAnywhere, Category="Spikes")
	float DownwardInterpSpeed = 1.f;
	
	UPROPERTY(EditAnywhere, Category="Spikes")
	float UpwardSpeed = 150.f;
	
	UPROPERTY(EditAnywhere, Category="Spikes")
	float UpwardInterpSpeed = 1.f;
	
	UPROPERTY(EditAnywhere, Category="Spikes")
	float TargetTolerance = 5.f;
	
	UPROPERTY(EditAnywhere, Category="Spikes")
	float UpPositionTime = 1.f;
	
	UPROPERTY(EditAnywhere, Category="Spikes")
	float DownPositionTime = .5f;
	
	UPROPERTY(EditDefaultsOnly, Category="Spikes")
	TObjectPtr<USplineComponent> RouteSpline;

	UPROPERTY(EditAnywhere, Category="Spikes")
	EInterpMethod InterpMethod = EInterpMethod::Default;

	UFUNCTION(BlueprintCallable)
	FORCEINLINE EMovementDirection GetMovementDirection() const { return MovementDirection; } 

private:
	FTimerHandle UpPositionTimer;
	FTimerHandle DownPositionTimer;
	FVector CurrentTarget = FVector::Zero();
	FVector UpPosition = FVector::ZeroVector;
	FVector DownPosition = FVector::ZeroVector;
	EMovementDirection MovementDirection = EMovementDirection::Still;
};
