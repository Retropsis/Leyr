// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "PassiveInfo.generated.h"

USTRUCT(BlueprintType)
struct FBasePassiveInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(Categories="Abilities"))
	FGameplayTag AbilityTag = FGameplayTag();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(Categories="Abilities.Type"))
	FGameplayTag AbilityType = FGameplayTag();

	UPROPERTY(BlueprintReadOnly)
	FGameplayTag StatusTag = FGameplayTag();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<const UTexture2D> Icon = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<const UMaterialInterface> BackgroundMaterial = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FGameplayModifierInfo> Modifiers;
	
	// UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	// TSubclassOf<UGameplayAbility> Ability;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 LevelRequirement = 1;
};

/**
 * 
 */
UCLASS()
class LEYR_API UPassiveInfo : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AbilityInformation")
	TArray<FBasePassiveInfo> PassiveInformation;

	FBasePassiveInfo FindPassiveInfoForTag(const FGameplayTag& AbilityTag, bool bLogNotFound = false) const;	
};
