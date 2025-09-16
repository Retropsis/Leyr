// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "CharacterInfo.generated.h"

class UAbilitySet;
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
struct FCharacterDefaultInfo
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, Category = "Assets")
	TObjectPtr<UNiagaraSystem> ImpactEffect;
	
	UPROPERTY(EditDefaultsOnly, Category="Assets")
	TMap<FGameplayTag, UNiagaraSystem*> WoundImpactEffects;
	
	UPROPERTY(EditDefaultsOnly, Category = "Assets")
	TObjectPtr<USoundBase> DeathSound;
	
	UPROPERTY(EditDefaultsOnly, Category = "Assets")
	TObjectPtr<UPaperZDAnimSequence> HitReactSequence;

	UPROPERTY(EditDefaultsOnly, Category = "Assets")
	TObjectPtr<UAttackSequenceInfo> AttackSequenceInfo;
};

UCLASS()
class LEYR_API UCharacterInfo : public UDataAsset
{
	GENERATED_BODY()

public:	
	UPROPERTY(EditDefaultsOnly, Category = "Character Defaults", meta=(TitleProperty=Key, Categories=Player))
	TMap<FGameplayTag, FCharacterDefaultInfo> CharacterInformation;

	UPROPERTY(EditDefaultsOnly, Category = "Character Defaults")
	TObjectPtr<UAbilitySet> AbilitySet;

	FCharacterDefaultInfo GetCharacterDefaultInfo(FGameplayTag CharacterTag);
};
