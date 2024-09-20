// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "AttackSequenceInfo.generated.h"

struct FGameplayTag;
struct FTaggedMontage;

/**
 * 
 */
UCLASS()
class LEYR_API UAttackSequenceInfo : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AttackSequenceInformation")
	TArray<FTaggedMontage> OneHandedSequences;

	FTaggedMontage FindSequenceInfoForTag(const FGameplayTag& MontageTag, bool bLogNotFound = false) const;
};
