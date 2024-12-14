// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "CharacterClassInfo.h"
#include "Engine/DataAsset.h"
#include "EncounterInfo.generated.h"

class UGameplayAbility;
class UGameplayEffect;
/**
 * 
 */

USTRUCT(BlueprintType)
struct FEncounterDefaultInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "EncounterD Defaults")
	FName Name = FName();
	
	UPROPERTY(EditDefaultsOnly, Category = "EncounterD Defaults")
	TSubclassOf<UGameplayEffect> PrimaryAttributes;
	
	UPROPERTY(EditDefaultsOnly, Category = "EncounterD Defaults")
	TSubclassOf<UGameplayEffect> SecondaryAttributes;

	UPROPERTY(EditDefaultsOnly, Category = "EncounterD Defaults")
	TSubclassOf<UGameplayEffect> VitalAttributes;
	
	UPROPERTY(EditDefaultsOnly, Category = "EncounterD Defaults")
	TArray<TSubclassOf<UGameplayAbility>> EncounterAbilities;
	
	UPROPERTY(EditDefaultsOnly, Category = "EncounterD Defaults")
	FScalableFloat XPReward = FScalableFloat();
};

UCLASS()
class LEYR_API UEncounterInfo : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "Encounter Defaults")
	TMap<FName, FEncounterDefaultInfo> EncounterInformation;

	UPROPERTY(EditDefaultsOnly, Category = "Encounter Defaults")
	TSubclassOf<UGameplayEffect> SecondaryAttributes;

	UPROPERTY(EditDefaultsOnly, Category = "Encounter Defaults")
	TSubclassOf<UGameplayEffect> VitalAttributes;
	
	UPROPERTY(EditDefaultsOnly, Category = "Encounter Defaults")
	TArray<TSubclassOf<UGameplayAbility>> CommonAbilities;
	
	UPROPERTY(EditDefaultsOnly, Category = "Encounter Defaults|Damage")
	TObjectPtr<UCurveTable> DamageCalculationCoefficients;

	FEncounterDefaultInfo GetEncounterDefaultInfo(FName EncounterName);
};
