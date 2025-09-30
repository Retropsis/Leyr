// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "World/Level/Zone/Arena.h"
#include "AIInterface.generated.h"

class ASplineComponentActor;
struct FGameplayTag;
class UPaperZDAnimInstance;
enum class EEncounterSize : uint8;
enum class EMovementType : uint8;
enum class EBehaviourState : uint8;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEndTimelineMovement);

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
	/*
	 * Components / Classes
	 */	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	ASplineComponentActor* FindSplineForTagName(const FName& TagName);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UPaperZDAnimInstance* FindPaperAnimInstanceForTagName(const FName& TagName);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UAnimInstance* FindMultiPartAnimInstanceForTagName(const FName& TagName);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ChangePaperAnimInstance(TSubclassOf<UPaperZDAnimInstance> NewInstance);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void FindAndApplyPatternParamsForPattern(const FName& PatternName);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UPaperZDAnimSequence* GetDespawnSequence();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Despawn();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void DespawnFinished();
	
	/*
	 * Movement
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void StartSplineMovement();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool FollowSplinePoints(int32 SplineIndex);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FVector FindRandomLocation();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SineMove();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void StartTimelineMovement();
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void TimelineMovement();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetJumpApex(float NewApex);
	
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
	FBoundLocations GetSpawningBounds();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetSpawningBounds(FBoundLocations NewBounds);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool IsWithinBounds(const FVector& Location);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool ShouldDespawn();

	/*
	 * Behaviour Tasks
	*/	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetupFlyAroundTarget(FVector TargetLocation, float Radius);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void FlyAroundTarget();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool ChaseTarget(AActor* TargetToChase);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool ChaseTargetWithinWater(AActor* TargetToChase);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool IsTargetWithinEnteringBounds(const FVector& Location);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool MoveToLocation(FVector TargetLocation, float Threshold, bool bBackward = false, bool bIgnoreZ = true);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FVector FindTargetLocation(AActor* TargetActor, float DistanceToKeep);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool CheckForObstacle(float TraceDistance, bool bBackward = false);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool CheckForGround(float TraceDistance, bool bBackward = false);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool RequestJump();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetOwningSummoner(AActor* NewSummoner);
};
