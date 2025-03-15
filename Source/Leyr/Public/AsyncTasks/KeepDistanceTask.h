// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "Tasks/AITask.h"
#include "KeepDistanceTask.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMoveComplete);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMoveFailed);

class AAIController;
/**
 * 
 */
UCLASS()
class LEYR_API UKeepDistanceTask : public UAITask
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, meta=(AdvancedDisplay="AcceptanceRadius", /*DefaultToSelf="Controller",*/ BlueprintInternalUseOnly="TRUE", DisplayName="Keep Distance From Location or Actor"))
	static UKeepDistanceTask* KeepDistance(AAIController* Controller, APawn* ControlledPawn, FVector TargetLocation, AActor* TargetActor = nullptr, float AcceptanceRadius = -1.f, bool bBackward = false);

	UPROPERTY(BlueprintAssignable)
	FOnMoveComplete OnMoveComplete;
	
	UPROPERTY(BlueprintAssignable)
	FOnMoveFailed OnMoveFailed;

protected:
	virtual void Activate() override;

private:
	TObjectPtr<AAIController> Controller = nullptr;
	UPROPERTY() TObjectPtr<APawn> ControlledPawn = nullptr;
	FVector TargetLocation = FVector::ZeroVector;
	UPROPERTY() TObjectPtr<AActor> TargetActor = nullptr;
	float AcceptanceRadius = 5.f;
	bool bBackward = false;
};
