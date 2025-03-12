// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "AI/AIData.h"
#include "Engine/DataAsset.h"
#include "BehaviourData.generated.h"

class UBehaviorTree;
enum class EBehaviourType : uint8;
/**
 * 
 */
UCLASS()
class LEYR_API UBehaviourData : public UDataAsset
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UBehaviorTree> BehaviorTree = nullptr;
	
	UPROPERTY(EditDefaultsOnly) // AI flow chart: patrol, airborne, ranger, turret
	EBehaviourType BehaviourType = EBehaviourType::Patrol;
	
	UPROPERTY(EditDefaultsOnly)
	bool bEditValues = false;	

	UPROPERTY(EditDefaultsOnly, meta=(EditCondition="bEditValues", EditConditionHides))
	float SineMoveHeight = 0.f;
	
	UPROPERTY(EditDefaultsOnly, meta=(EditCondition="bEditValues", EditConditionHides)) // How far to patrol from start location
	float PatrolRadius = 750.f;
	
	UPROPERTY(EditDefaultsOnly, meta=(EditCondition="bEditValues", EditConditionHides)) // How far it moves every step
	float PatrolTickRadius = 450.f;
	
	UPROPERTY(EditDefaultsOnly, meta=(EditCondition="bEditValues", EditConditionHides)) // Distance to reach for attacking
	float AttackRange = 300.f;
	
	UPROPERTY(EditDefaultsOnly, meta=(EditCondition="bEditValues", EditConditionHides)) // How long before attacking again
	float AttackCooldown = 5.f;
	
	UPROPERTY(EditDefaultsOnly, meta=(EditCondition="bEditValues", EditConditionHides)) // Distance to reach until it falls back
	float CloseRange = 275.f;
	
	UPROPERTY(EditDefaultsOnly, meta=(EditCondition="bEditValues", EditConditionHides)) // Height offset to adjust and stay above player
	float ChasingHeightOffset = 75.f;
	
	UPROPERTY(EditDefaultsOnly, meta=(EditCondition="bEditValues", EditConditionHides)) // How fast to chase the player
	float ChasingSpeed = 250.f;
	
	UPROPERTY(EditDefaultsOnly, meta=(EditCondition="bEditValues", EditConditionHides)) // How fast to dive
	float DivingSpeed = 750.f;
	
	UPROPERTY(EditDefaultsOnly)
	bool bCollisionCauseDamage = false;

	UPROPERTY(EditDefaultsOnly, meta=(EditCondition="bCollisionCauseDamage", EditConditionHides))
	FScalableFloat AbilityPower;
	
	UPROPERTY(EditDefaultsOnly, meta=(EditCondition="bCollisionCauseDamage", EditConditionHides))
	TSubclassOf<UGameplayEffect> DamageEffectClass;
	
	UPROPERTY(EditDefaultsOnly, meta=(Categories="Damage", EditCondition="bCollisionCauseDamage", EditConditionHides))
	FGameplayTag DamageType;
	
	UPROPERTY(EditDefaultsOnly, meta=(EditCondition="bCollisionCauseDamage", EditConditionHides))
	bool bShouldApplyInvincibility = false;	
};
