// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "AI/AIData.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "EncounterData.generated.h"

enum class ECharacterClass : uint8;
class UAttributeData;
class ULootDataSet;
class UAbilitySet;
class AAICharacter;
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
	TSoftClassPtr<AAICharacter> EncounterClass = nullptr;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAbilitySet> AbilitySet;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAttributeData> AttributeData;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UBehaviourData> BehaviourData = nullptr;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<ULootDataSet> LootData = nullptr;

	UPROPERTY(EditDefaultsOnly)
	bool bShouldDespawn = true;
	
	UPROPERTY(EditDefaultsOnly)
	bool bSimulatePhysicsOnDestroyed = true;
	
	UPROPERTY(EditDefaultsOnly) // Encounter Size for Max Health and other purposes ?
	EEncounterSize EncounterSize = EEncounterSize::Default;
	
	UPROPERTY(EditDefaultsOnly)
	ECharacterClass CharacterClass = ECharacterClass::Warrior;

	UPROPERTY(EditDefaultsOnly)
	FScalableFloat XPReward = FScalableFloat();

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag WeaponSocketTag = FGameplayTag();
	
	UPROPERTY(EditDefaultsOnly, Category="Asset")
	TSoftObjectPtr<UNiagaraSystem> ImpactEffect;
	
	UPROPERTY(EditDefaultsOnly, Category="Asset")
	TSoftObjectPtr<UNiagaraSystem> DestroyedEffect;
	
	UPROPERTY(EditDefaultsOnly, Category="Asset")
	TSoftObjectPtr<USoundBase> DeathSound;
	
	UPROPERTY(EditDefaultsOnly, Category="Asset")
	TSoftObjectPtr<UPaperZDAnimSequence> HitReactSequence;
	
	UPROPERTY(EditDefaultsOnly, Category="Asset")
	TObjectPtr<UAttackSequenceInfo> AttackSequenceInfo;
};
