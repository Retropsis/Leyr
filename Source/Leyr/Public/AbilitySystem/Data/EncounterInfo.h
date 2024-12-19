// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "AI/AICharacter.h"
#include "Engine/DataAsset.h"
#include "EncounterInfo.generated.h"

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
	TMap<FName, FEncounterDefaultInfo> EncounterInformation;

	UPROPERTY(EditDefaultsOnly, Category = "Encounter Defaults")
	TSubclassOf<UGameplayEffect> SecondaryAttributes;

	UPROPERTY(EditDefaultsOnly, Category = "Encounter Defaults")
	TSubclassOf<UGameplayEffect> VitalAttributes;
	
	UPROPERTY(EditDefaultsOnly, Category = "Encounter Defaults")
	TArray<TSubclassOf<UGameplayAbility>> CommonAbilities;
	
	UPROPERTY(EditDefaultsOnly, Category = "Encounter Defaults|Damage")
	TObjectPtr<UCurveTable> DamageCalculationCoefficients;

	FEncounterDefaultInfo GetEncounterDefaultInfo(FName EncounterName);
};
