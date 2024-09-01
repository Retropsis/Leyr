// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "World/Level/Platform.h"
#include "MovingPlatform.generated.h"

class USplineComponent;
/**
 * 
 */
UCLASS()
class LEYR_API AMovingPlatform : public APlatform
{
	GENERATED_BODY()

public:
	AMovingPlatform();
	
	UFUNCTION(BlueprintCallable)
	void ChooseNextTarget();

	UFUNCTION(BlueprintCallable)
	bool HasReachedTarget() const;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void MoveTo();
	
	UPROPERTY(EditDefaultsOnly, Category="Platform|Route")
	float TargetTolerance = 5.f;
	
	UPROPERTY(EditDefaultsOnly, Category="Platform|Route")
	TObjectPtr<USplineComponent> RouteSpline;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly)
	FVector CurrentTarget = FVector::Zero();
	
	int32 CurrentIndex = 0;
};
