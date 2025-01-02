// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CharacterInfo.generated.h"

class UAttackSequenceInfo;
class UPaperZDAnimSequence;
class UNiagaraSystem;
class UBehaviorTree;
class UGameplayAbility;
class UGameplayEffect;
/**
 * 
*/

UENUM(BlueprintType)
enum class ECharacterName : uint8
{
	FirstCharacter,
	SecondCharacter,
};

USTRUCT(BlueprintType)
struct FCharacterDefaultInfo
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, Category = "Behaviour")
	TObjectPtr<UNiagaraSystem> ImpactEffect;
	
	UPROPERTY(EditDefaultsOnly, Category = "Behaviour")
	TObjectPtr<USoundBase> DeathSound;
	
	UPROPERTY(EditDefaultsOnly, Category = "Behaviour")
	TObjectPtr<UPaperZDAnimSequence> HitReactSequence;

	UPROPERTY(EditDefaultsOnly, Category = "Behaviour")
	TObjectPtr<UAttackSequenceInfo> AttackSequenceInfo;
};

UCLASS()
class LEYR_API UCharacterInfo : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "Character Defaults")
	TMap<ECharacterName, FCharacterDefaultInfo> CharacterInformation;

	UPROPERTY(EditDefaultsOnly, Category = "Character Defaults")
	TSubclassOf<UGameplayEffect> SecondaryAttributes;

	UPROPERTY(EditDefaultsOnly, Category = "Character Defaults")
	TSubclassOf<UGameplayEffect> VitalAttributes;
	
	UPROPERTY(EditDefaultsOnly, Category = "Character Defaults")
	TArray<TSubclassOf<UGameplayAbility>> CommonAbilities;

	FCharacterDefaultInfo GetCharacterDefaultInfo(ECharacterName CharacterName);
};
