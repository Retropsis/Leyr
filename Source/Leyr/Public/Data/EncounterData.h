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

	UPROPERTY(EditDefaultsOnly, Category="Data")
	TObjectPtr<UAbilitySet> AbilitySet;
	
	UPROPERTY(EditDefaultsOnly, Category="Data")
	TObjectPtr<UAttributeData> AttributeData;

	UPROPERTY(EditDefaultsOnly, Category="Data")
	TObjectPtr<UBehaviourData> BehaviourData = nullptr;

	UPROPERTY(EditDefaultsOnly, Category="Setup")
	bool bShouldDespawn = true;

	UPROPERTY(EditDefaultsOnly, Category="Setup", meta=(EditCondition="bShouldDespawn"))
	TSoftObjectPtr<UPaperZDAnimSequence> DespawnSequence = nullptr;
	
	UPROPERTY(EditDefaultsOnly, Category="Setup")
	bool bSimulatePhysicsOnDestroyed = true;
	
	UPROPERTY(EditDefaultsOnly, Category="Setup") // Encounter Size for Max Health and other purposes ?
	EEncounterSize EncounterSize = EEncounterSize::Default;
	
	UPROPERTY(EditDefaultsOnly, Category="Setup")
	ECharacterClass CharacterClass = ECharacterClass::Warrior;

	UPROPERTY(EditDefaultsOnly, Category="Reward")
	FScalableFloat XPReward = FScalableFloat();
	
	UPROPERTY(EditDefaultsOnly, Category="Reward")
	TObjectPtr<ULootDataSet> LootData = nullptr;

	UPROPERTY(EditDefaultsOnly, Category="VFX / SFX")
	FGameplayTag WeaponSocketTag = FGameplayTag();
	
	UPROPERTY(EditDefaultsOnly, Category="VFX / SFX")
	TMap<FGameplayTag, TSoftObjectPtr<UNiagaraSystem>> WoundImpactEffects;
	
	UPROPERTY(EditDefaultsOnly, Category="VFX / SFX")
	TMap<FGameplayTag, TSoftObjectPtr<UPaperZDAnimSequence>> HitReactSequences;
	
	UPROPERTY(EditDefaultsOnly, Category="VFX / SFX")
	TSoftObjectPtr<UNiagaraSystem> DestroyedEffect;
	
	UPROPERTY(EditDefaultsOnly, Category="VFX / SFX")
	TSoftObjectPtr<USoundBase> DeathSound;
	
	UPROPERTY(EditDefaultsOnly, Category="VFX / SFX")
	TObjectPtr<UAttackSequenceInfo> AttackSequenceInfo;

	UPROPERTY(EditDefaultsOnly, Category="Editor")
	TObjectPtr<UTexture2D> EncounterIcon = nullptr;
};
