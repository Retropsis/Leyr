// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "AbilitySystem/Data/AbilityDescriptionData.h"
#include "Engine/DataAsset.h"
#include "AbilitySet.generated.h"

class UAbilityData;
struct FActiveGameplayEffectHandle;
struct FGameplayAbilitySpecHandle;
class UGameplayEffect;
class UGameplayAbility;
class UAbilitySystemComponent;

/**
 * FAbilitySet_GameplayAbility
 *
 *	Data used by the ability set to grant gameplay abilities.
 */
USTRUCT(BlueprintType)
struct FAbilitySet_GameplayAbility
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayAbility> Ability = nullptr;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAbilityData> AbilityData = nullptr;

	UPROPERTY(EditDefaultsOnly)
	int32 Level = 1;
	
	UPROPERTY(EditDefaultsOnly)
	int32 LevelRequirement = 1;

	UPROPERTY(EditDefaultsOnly, Meta=(Categories = "InputTag"))
	FGameplayTag InputTag = FGameplayTag();

	UPROPERTY(EditDefaultsOnly, meta=(Categories="Abilities"))
	FGameplayTag AbilityTag = FGameplayTag();
	
	UPROPERTY(EditDefaultsOnly, meta=(Categories="Abilities.Type"))
	FGameplayTag AbilityType = FGameplayTag();

	UPROPERTY(EditDefaultsOnly, meta=(Categories="Cooldown"))
	FGameplayTag CooldownTag = FGameplayTag();

	UPROPERTY(BlueprintReadOnly)
	FGameplayTag StatusTag = FGameplayTag();

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<const UTexture2D> Icon = nullptr;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<const UTexture2D> LockedIcon = nullptr;

	UPROPERTY(EditDefaultsOnly)
	FAbilityDescription Descriptions = FAbilityDescription();
};


/**
 * FAbilitySet_GameplayEffect
 *
 *	Data used by the ability set to grant gameplay effects.
 */
USTRUCT(BlueprintType)
struct FAbilitySet_GameplayEffect
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> GameplayEffect = nullptr;

	UPROPERTY(EditDefaultsOnly)
	float Level = 1.0f;
};

UENUM()
enum class ETagOperation
{
	Reset,
	Add,
	Remove,
	Replace,
};

/**
 * FAbilitySet_GrantedHandles
 *
 *	Data used to store handles to what has been granted by the ability set.
 */
USTRUCT(BlueprintType)
struct FAbilitySet_GrantedHandles
{
	GENERATED_BODY()

public:
	void AddAbilitySpecHandle(const FGameplayAbilitySpecHandle& Handle);
	void AddGameplayEffectHandle(const FActiveGameplayEffectHandle& Handle);
	void TakeFromAbilitySystem(UAbilitySystemComponent* ASC);
	void UpdateInputTags(const UAbilitySystemComponent* ASC, const FGameplayTagContainer& InputTags, ETagOperation TagOperation = ETagOperation::Replace);

protected:
	UPROPERTY()
	TArray<FGameplayAbilitySpecHandle> AbilitySpecHandles;

	UPROPERTY()
	TArray<FActiveGameplayEffectHandle> GameplayEffectHandles;
};

/**
 * 
 */
UCLASS()
class LEYR_API UAbilitySet : public UDataAsset
{
	GENERATED_BODY()

public:
	void GiveToAbilitySystem(UAbilitySystemComponent* ASC, FAbilitySet_GrantedHandles* OutGrantedHandles, FGameplayTag InputTag = FGameplayTag(), float Level = 1.f, UObject* SourceObject = nullptr) const;
	
	UPROPERTY(EditDefaultsOnly, Category="Gameplay Abilities", meta=(Categories="Abilities"))
	FGameplayTag AbilityTag = FGameplayTag();

protected:
	UPROPERTY(EditDefaultsOnly, Category="Gameplay Abilities", meta=(TitleProperty=Ability))
	TArray<FAbilitySet_GameplayAbility> GrantedGameplayAbilities;

	// These abilities are always loaded and not saved, non-UI, necessary gameplay, level 1
	UPROPERTY(EditDefaultsOnly, Category="Gameplay Abilities", meta=(TitleProperty=Ability))
	TArray<TSubclassOf<UGameplayAbility>> CommonGameplayAbilities;

	UPROPERTY(EditDefaultsOnly, Category="Gameplay Effects", meta=(TitleProperty=GameplayEffect))
	TArray<FAbilitySet_GameplayEffect> GrantedGameplayEffects;
};
