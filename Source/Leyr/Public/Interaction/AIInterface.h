// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AIInterface.generated.h"

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

	virtual EBehaviourState GetBehaviourState() = 0;
	virtual void SetBehaviourState(EBehaviourState NewState) = 0;
};
