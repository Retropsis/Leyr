// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "AI/AICharacter.h"
#include "Engine/DataAsset.h"
#include "BehaviourInfo.generated.h"

/**
 * 
*/

class UBehaviorTree;
enum class EMovementType : uint8;

USTRUCT(BlueprintType)
struct FBehaviourDefaultInfo
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, Category = "Behaviour")
	TObjectPtr<UBehaviorTree> BehaviorTree;
	
	UPROPERTY(EditDefaultsOnly, Category = "Behaviour") // AI flow chart: patrol, airborne, ranger, turret
	EBehaviourType BehaviourType = EBehaviourType::Patrol;

	UPROPERTY(EditDefaultsOnly, Category = "Behaviour") // Moving logic, maybe spawn spline if needed
	EMovementType MovementType = EMovementType::Destination;

	UPROPERTY(EditDefaultsOnly, Category = "Behaviour")
	float SineMoveHeight = 0.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Behaviour") // How far to patrol from start location
	float PatrolRadius = 750.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Behaviour") // How far it moves every step
	float PatrolTickRadius = 450.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Behaviour") // Distance to reach for attacking
	float AttackRange = 300.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Behaviour") // Distance to reach until it falls back
	float CloseRange = 275.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Behaviour") // Height offset to adjust and stay above player
	float ChasingHeightOffset = 75.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Behaviour")
	bool bCollisionCauseDamage = false;
};

UCLASS()
class LEYR_API UBehaviourInfo : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "Behaviour")
	TMap<FName, FBehaviourDefaultInfo> BehaviourInformation;

	FBehaviourDefaultInfo GetBehaviourDefaultInfo(FName BehaviourName);
};
