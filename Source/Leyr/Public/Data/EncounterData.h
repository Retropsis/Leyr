// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "AI/AIData.h"
#include "Engine/DataAsset.h"
#include "EncounterData.generated.h"

class UPaperZDAnimSequence;
class UNiagaraSystem;
class UAttackSequenceInfo;
class UBehaviourData;
/**
 * 
 */
UCLASS(BlueprintType)
class LEYR_API UEncounterData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> PrimaryAttributes;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> SecondaryAttributes;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> VitalAttributes;

	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<UGameplayAbility>> Abilities;
	
	UPROPERTY(EditDefaultsOnly) // Encounter Size for Max Health and other purposes ?
	EEncounterSize EncounterSize = EEncounterSize::Default;

	UPROPERTY(EditDefaultsOnly)
	FScalableFloat XPReward = FScalableFloat();

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UBehaviourData> DefaultBehaviourData = nullptr;
	
	UPROPERTY(EditDefaultsOnly, Category="Asset")
	TObjectPtr<UNiagaraSystem> ImpactEffect;
	
	UPROPERTY(EditDefaultsOnly, Category="Asset")
	TObjectPtr<USoundBase> DeathSound;
	
	UPROPERTY(EditDefaultsOnly, Category="Asset")
	TObjectPtr<UPaperZDAnimSequence> HitReactSequence;

	UPROPERTY(EditDefaultsOnly, Category="Asset")
	TObjectPtr<UAttackSequenceInfo> AttackSequenceInfo;
};
