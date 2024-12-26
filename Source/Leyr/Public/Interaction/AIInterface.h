// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AIInterface.generated.h"

enum class EMovementType : uint8;
enum class EBehaviourState : uint8;
// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UAIInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class LEYR_API IAIInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FVector FindRandomLocation();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool MoveToLocation(FVector TargetLocation, float Threshold);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool ChaseTarget(AActor* TargetToChase);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SineMove();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool FollowSpline(int32 SplineIndex);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FVector GetNextLocation(int32 SplineIndex);

	virtual EBehaviourState GetBehaviourState() = 0;
	virtual void SetBehaviourState(EBehaviourState NewState) = 0;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	EMovementType GetMovementType();
};
