// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "DescriptionInfo.generated.h"

class UGameplayAbility;

USTRUCT(BlueprintType)
struct FBaseDescriptionInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag DescriptionTag = FGameplayTag();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText Description = FText();
};

/**
 * 
 */
UCLASS()
class LEYR_API UDescriptionInfo : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(TitleProperty="DescriptionTag"))
	TArray<FBaseDescriptionInfo> DescriptionInformation;

	UFUNCTION(BlueprintPure, meta=(AdvancedDisplay=1))
	FBaseDescriptionInfo FindDescriptionInfoForTag(const FGameplayTag& DescriptionTag, bool bLogNotFound = false) const;
};
