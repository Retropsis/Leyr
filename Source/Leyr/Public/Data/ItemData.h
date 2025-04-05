// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "CombatData.h"
#include "GameplayEffect.h"
#include "GameplayTagContainer.h"
#include "AbilitySystem/Data/AttackSequenceInfo.h"
#include "Engine/DataAsset.h"
#include "ItemData.generated.h"

class UAbilitySet;
class UAbilityData;
class UPaperZDAnimInstance;
class UPaperFlipbook;
class UGameplayEffect;
class AItem;

UENUM(BlueprintType)
enum class EItemType : uint8
{
	Misc UMETA(DisplayName="Misc"),
	Resource UMETA(DisplayName="Resource"),
	Equipment UMETA(DisplayName="Equipment"),
	Armor UMETA(DisplayName="Armor"),
	Consumable UMETA(DisplayName="Consumable"),
};

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class LEYR_API UItemData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Item")
	int32 ID = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Item")
	TObjectPtr<UTexture2D> Icon = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Item")
	TObjectPtr<UPaperFlipbook> PickupFlipbook = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Item")
	TObjectPtr<USoundBase> PickupSound = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Item")
	FText Name = FText();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Item")
	FText Description = FText();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Item")
	bool bIsStackable = false;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Item")
	bool bRemoveStackIfEmpty = false;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Item")
	int32 StackSize = 0;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Item")
	EItemType ItemType = EItemType::Misc;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Item", meta=(Categories="Equipment"))
	FGameplayTag EquipmentSlot = FGameplayTag();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Item")
	float Weight = 0.f;
	
	/*
	 * Abilities
	*/
	UPROPERTY(EditDefaultsOnly, Category="Ability")
	TObjectPtr<UAbilitySet> AbilitySet;
	
	UPROPERTY(EditDefaultsOnly, Category="Ability")
	UAbilityData* AbilityData;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Item", meta=(Categories="Abilities"))
	TArray<FGameplayTag> Abilities;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Item")
	TArray<TSubclassOf<UGameplayEffect>> Effects;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Item")
	TArray<FGameplayModifierInfo> Modifiers;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Item", meta=(Categories="Abilities"))
	FGameplayTag MasteryTag = FGameplayTag();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Item", meta=(Categories="Cost"))
	FGameplayTag CostTag = FGameplayTag();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Animation")
	TArray<FTaggedMontage> DefaultSequences;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Animation")
	ESequenceType SequenceType = ESequenceType::Default;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Animation")
	TSubclassOf<UPaperZDAnimInstance> AnimationInstance = nullptr;
	/*
	 *
	 */

	UFUNCTION()
	FTaggedMontage FindSequenceInfoForTag(const FGameplayTag& MontageTag, bool bLogNotFound = false) const;

	UFUNCTION(BlueprintPure, Category="Item")
	float GetPrimaryAttributeMagnitude();
};
