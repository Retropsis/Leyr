// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "World/Level/Zone/Arena.h"
#include "AIInterface.generated.h"

enum class EEncounterSize : uint8;
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
	void StartSplineMovement();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool FollowSplinePoints(int32 SplineIndex);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FVector FindRandomLocation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SineMove();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void FaceTarget();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool FollowSpline(int32 SplineIndex);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FVector GetNextLocation(int32 SplineIndex);

	virtual EBehaviourState GetBehaviourState() = 0;
	virtual void SetBehaviourState(EBehaviourState NewState) = 0;
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	EEncounterSize GetEncounterSize();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	EMovementType GetMovementType();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetNewMovementSpeed(EMovementMode InMovementMode, float NewSpeed);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FName GetNextBehaviourPattern(FName PatternName);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FBoundLocations GetArenaBounds();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FBoxSphereBounds GetEnteringBounds();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FBoxSphereBounds GetNavigationBounds();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool IsWithinBounds(const FVector& Location);

	/*
	 * Behaviour Tasks
	*/	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool ChaseTarget(AActor* TargetToChase);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool ChaseTargetWithinWater(AActor* TargetToChase);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool IsTargetWithinEnteringBounds(const FVector& Location);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool MoveToLocation(FVector TargetLocation, float Threshold, bool bBackward = false);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FVector FindTargetLocation(AActor* TargetActor, float DistanceToKeep);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool CheckForObstacle(float TraceDistance, bool bBackward = false);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool CheckForGround(float TraceDistance, bool bBackward = false);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool RequestJump();
};
