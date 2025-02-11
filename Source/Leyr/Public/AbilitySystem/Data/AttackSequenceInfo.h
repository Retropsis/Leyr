// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "Data/CombatData.h"
#include "Engine/DataAsset.h"
#include "AttackSequenceInfo.generated.h"

struct FGameplayTag;

UENUM(BlueprintType)
enum class ESequenceType : uint8
{
	Default,
	MartialArt,
	OneHanded,
	TwoHanded,
	Use,
	Throw,
	Dagger,
	Piercing,
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
	TArray<FTaggedMontage> TwoHandedSequences;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AttackSequenceInformation", meta=(TitleProperty="MontageTag"))
	FTaggedMontage UseSequence;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AttackSequenceInformation", meta=(TitleProperty="MontageTag"))
	FTaggedMontage ThrowSequence;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AttackSequenceInformation", meta=(TitleProperty="MontageTag"))
	TArray<FTaggedMontage> DaggerSequences;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AttackSequenceInformation", meta=(TitleProperty="MontageTag"))
	TArray<FTaggedMontage> PiercingSequences;

	FTaggedMontage FindSequenceInfoForTag(const FGameplayTag& MontageTag, ESequenceType SequenceType, bool bLogNotFound = false) const;
	TArray<FTaggedMontage> GetSequencesByType(ESequenceType SequenceType);
};
