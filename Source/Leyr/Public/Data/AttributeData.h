// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "Engine/DataAsset.h"
#include "AttributeData.generated.h"

USTRUCT(Blueprintable, BlueprintType)
struct FAttribute
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	EGameplayEffectMagnitudeCalculation MagnitudeCalculation = EGameplayEffectMagnitudeCalculation::ScalableFloat;

	UPROPERTY(EditDefaultsOnly, meta=(EditCondition="MagnitudeCalculation==EGameplayEffectMagnitudeCalculation::ScalableFloat", EditConditionHides))
	FScalableFloat Value = 1.f;
	
	UPROPERTY(EditDefaultsOnly, meta=(EditCondition="MagnitudeCalculation==EGameplayEffectMagnitudeCalculation::AttributeBased || MagnitudeCalculation==EGameplayEffectMagnitudeCalculation::CustomCalculationClass", EditConditionHides))
	FScalableFloat Coefficient = 0.f;
	
	UPROPERTY(EditDefaultsOnly, meta=(EditCondition="MagnitudeCalculation==EGameplayEffectMagnitudeCalculation::AttributeBased || MagnitudeCalculation==EGameplayEffectMagnitudeCalculation::CustomCalculationClass", EditConditionHides))
	FScalableFloat PreMultiplyAdditiveValue = 0.f;
	
	UPROPERTY(EditDefaultsOnly, meta=(EditCondition="MagnitudeCalculation==EGameplayEffectMagnitudeCalculation::AttributeBased || MagnitudeCalculation==EGameplayEffectMagnitudeCalculation::CustomCalculationClass", EditConditionHides))
	FScalableFloat PostMultiplyAdditiveValue = 0.f;
	
	UPROPERTY(EditDefaultsOnly, meta=(EditCondition="MagnitudeCalculation==EGameplayEffectMagnitudeCalculation::AttributeBased", EditConditionHides))
	FGameplayAttribute BackingAttribute;
	
	UPROPERTY(EditDefaultsOnly, meta=(EditCondition="MagnitudeCalculation==EGameplayEffectMagnitudeCalculation::CustomCalculationClass", EditConditionHides))
	TSubclassOf<UGameplayModMagnitudeCalculation> CalculationClassMagnitude = nullptr;
};

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class LEYR_API UAttributeData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	void BuildAttributes();
	void GiveToAbilitySystem(UAbilitySystemComponent* ASC, float Level);
	
	UPROPERTY(EditDefaultsOnly, DisplayName="HP (Health Points)", meta=(FullyExpand=true))
	FAttribute MaxHealth;
	
	UPROPERTY(EditDefaultsOnly, DisplayName="MP (Mana Points)", meta=(FullyExpand=true))
	FAttribute MaxMana;
	
	UPROPERTY(EditDefaultsOnly, DisplayName="STR (Strength)", meta=(FullyExpand=true))
	FAttribute Strength;
	
	UPROPERTY(EditDefaultsOnly, DisplayName="DEX (Dexterity)", meta=(FullyExpand=true))
	FAttribute Dexterity;
	
	UPROPERTY(EditDefaultsOnly, DisplayName="VIT (Vitality)", meta=(FullyExpand=true))
	FAttribute Vitality;
	
	UPROPERTY(EditDefaultsOnly, DisplayName="INT (Intelligence)", meta=(FullyExpand=true))
	FAttribute Intelligence;
	
	UPROPERTY(EditDefaultsOnly, DisplayName="WIS (Wisdom)", meta=(FullyExpand=true))
	FAttribute Wisdom;
	
	UPROPERTY(EditDefaultsOnly, DisplayName="SPR (Spirit)", meta=(FullyExpand=true))
	FAttribute Spirit;
	
	UPROPERTY(EditDefaultsOnly, DisplayName="LCK (Luck)", meta=(FullyExpand=true))
	FAttribute Luck;

private:
	TArray<FGameplayModifierInfo> Attributes;
	TArray<FGameplayModifierInfo> VitalAttributes;
};
