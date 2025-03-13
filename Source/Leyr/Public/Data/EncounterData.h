// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "AI/AIData.h"
#include "Engine/DataAsset.h"
#include "EncounterData.generated.h"

enum class ECharacterClass : uint8;
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
		
	UPROPERTY(EditDefaultsOnly, Category="Attributes")
	TSubclassOf<UGameplayEffect> PrimaryAttributes;

	UPROPERTY(EditDefaultsOnly, Category="Attributes")
	TSubclassOf<UGameplayEffect> SecondaryAttributes;

	UPROPERTY(EditDefaultsOnly, Category="Attributes")
	TSubclassOf<UGameplayEffect> VitalAttributes;

	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<UGameplayAbility>> Abilities;
	
	UPROPERTY(EditDefaultsOnly) // Encounter Size for Max Health and other purposes ?
	EEncounterSize EncounterSize = EEncounterSize::Default;
	
	UPROPERTY(EditDefaultsOnly)
	ECharacterClass CharacterClass = ECharacterClass::Warrior;

	UPROPERTY(EditDefaultsOnly)
	FScalableFloat XPReward = FScalableFloat();

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UBehaviourData> BehaviourData = nullptr;
	
	UPROPERTY(EditDefaultsOnly, Category="Asset")
	TSoftObjectPtr<UNiagaraSystem> ImpactEffect;
	
	UPROPERTY(EditDefaultsOnly, Category="Asset")
	TSoftObjectPtr<USoundBase> DeathSound;
	
	UPROPERTY(EditDefaultsOnly, Category="Asset")
	TSoftObjectPtr<UPaperZDAnimSequence> HitReactSequence;

	UPROPERTY(EditDefaultsOnly, Category="Asset")
	TObjectPtr<UAttackSequenceInfo> AttackSequenceInfo;
};
