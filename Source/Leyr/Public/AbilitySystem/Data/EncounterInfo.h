// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "Leyr/Leyr.h"
#include "AI/AIData.h"
#include "EncounterInfo.generated.h"

class UAttackSequenceInfo;
class UPaperZDAnimSequence;
class UNiagaraSystem;
class UBehaviorTree;
class UGameplayAbility;
class UGameplayEffect;
/**
 * 
*/

USTRUCT(BlueprintType)
struct FBehaviourDefaultInfo
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, Category = "Behaviour")
	TObjectPtr<UBehaviorTree> BehaviorTree;
	
	UPROPERTY(EditDefaultsOnly, Category = "Behaviour") // AI flow chart: patrol, airborne, ranger, turret
	EBehaviourType BehaviourType = EBehaviourType::Patrol;
	
	UPROPERTY(EditDefaultsOnly, Category = "Behaviour") // Encounter Size for Max Health and other purposes ?
	EEncounterSize EncounterSize = EEncounterSize::Default;

	UPROPERTY(EditDefaultsOnly, Category = "Behaviour")
	float SineMoveHeight = 0.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Behaviour") // How far to patrol from start location
	float PatrolRadius = 750.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Behaviour") // How far it moves every step
	float PatrolTickRadius = 450.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Behaviour") // Distance to reach for attacking
	float AttackRange = 300.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Behaviour") // How long before attacking again
	float AttackCooldown = 5.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Behaviour") // Distance to reach until it falls back
	float CloseRange = 275.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Behaviour") // Height offset to adjust and stay above player
	float ChasingHeightOffset = 75.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Behaviour") // How fast to chase the player
	float ChasingSpeed = 250.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Behaviour") // How fast to dive
	float DivingSpeed = 750.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Behaviour")
	bool bCollisionCauseDamage = false;
	
	UPROPERTY(EditDefaultsOnly, Category="Combat", meta=(Categories="Damage", EditCondition="bCollisionCauseDamage", EditConditionHides))
	bool bShouldApplyInvincibility = false;	

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Combat", meta=(EditCondition="bCollisionCauseDamage", EditConditionHides))
	FValueRange AbilityPower;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Combat", meta=(EditCondition="bCollisionCauseDamage", EditConditionHides))
	TSubclassOf<UGameplayEffect> DamageEffectClass;
	
	UPROPERTY(EditDefaultsOnly, Category="Combat", meta=(Categories="Damage", EditCondition="bCollisionCauseDamage", EditConditionHides))
	FGameplayTag DamageType;
	
	UPROPERTY(EditDefaultsOnly, Category = "Behaviour")
	TObjectPtr<UNiagaraSystem> ImpactEffect;
	
	UPROPERTY(EditDefaultsOnly, Category = "Behaviour")
	TObjectPtr<USoundBase> DeathSound;
	
	UPROPERTY(EditDefaultsOnly, Category = "Behaviour")
	TObjectPtr<UPaperZDAnimSequence> HitReactSequence;

	UPROPERTY(EditDefaultsOnly, Category = "Behaviour")
	TObjectPtr<UAttackSequenceInfo> AttackSequenceInfo;
};

USTRUCT(BlueprintType)
struct FEncounterDefaultInfo
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, Category = "Encounter Defaults")
	TSubclassOf<UGameplayEffect> PrimaryAttributes;
	
	UPROPERTY(EditDefaultsOnly, Category = "Encounter Defaults")
	TSubclassOf<UGameplayEffect> SecondaryAttributes;

	UPROPERTY(EditDefaultsOnly, Category = "Encounter Defaults")
	TSubclassOf<UGameplayEffect> VitalAttributes;
	
	UPROPERTY(EditDefaultsOnly, Category = "Encounter Defaults")
	TArray<TSubclassOf<UGameplayAbility>> EncounterAbilities;

	UPROPERTY(EditDefaultsOnly, Category = "Encounter Defaults")
	FBehaviourDefaultInfo BehaviourDefaultInfo;
	
	UPROPERTY(EditDefaultsOnly, Category = "Encounter Defaults")
	FScalableFloat XPReward = FScalableFloat();
};

UCLASS()
class LEYR_API UEncounterInfo : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "Encounter Defaults")
	TMap<EEncounterName, FEncounterDefaultInfo> EncounterInformation;
	
	UPROPERTY(EditDefaultsOnly, Category = "Encounter Defaults")
	TMap<FGameplayTag, FEncounterDefaultInfo> EncounterInformationMap;
	
	UPROPERTY(EditDefaultsOnly, Category = "Encounter Defaults")
	TSubclassOf<UGameplayEffect> PrimaryAttributes;

	UPROPERTY(EditDefaultsOnly, Category = "Encounter Defaults")
	TSubclassOf<UGameplayEffect> SecondaryAttributes;

	UPROPERTY(EditDefaultsOnly, Category = "Encounter Defaults")
	TSubclassOf<UGameplayEffect> VitalAttributes;
	
	UPROPERTY(EditDefaultsOnly, Category = "Encounter Defaults")
	TArray<TSubclassOf<UGameplayAbility>> CommonAbilities;

	FEncounterDefaultInfo GetEncounterDefaultInfo(EEncounterName EncounterName);
};
