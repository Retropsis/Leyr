// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "AttackSequenceInfo.generated.h"

struct FGameplayTag;
struct FTaggedMontage;

UENUM(BlueprintType)
enum class ESequenceType : uint8
{
	Default,
	MartialArt,
	OneHanded,
	TwoHanded,
};

/**
 * 
 */
UCLASS()
class LEYR_API UAttackSequenceInfo : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AttackSequenceInformation", meta=(TitleProperty="MontageTag"))
	TArray<FTaggedMontage> DefaultSequences;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AttackSequenceInformation", meta=(TitleProperty="MontageTag"))
	TArray<FTaggedMontage> MartialArtSequences;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AttackSequenceInformation", meta=(TitleProperty="MontageTag"))
	TArray<FTaggedMontage> OneHandedSequences;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AttackSequenceInformation", meta=(TitleProperty="MontageTag"))
	TArray<FTaggedMontage> TowHandedSequences;

	FTaggedMontage FindSequenceInfoForTag(const FGameplayTag& MontageTag, ESequenceType SequenceType, bool bLogNotFound = false) const;
	TArray<FTaggedMontage> GetSequencesByType(ESequenceType SequenceType);
};
