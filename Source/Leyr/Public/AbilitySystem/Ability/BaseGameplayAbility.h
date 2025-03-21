// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "AbilitySystem/Data/AttackSequenceInfo.h"
#include "Data/CombatData.h"
#include "Leyr/Leyr.h"
#include "BaseGameplayAbility.generated.h"

class UNiagaraSystem;
class UGameplayCueDefinition;
class UAbilityData;
class UPaperZDAnimInstance;
class UItemData;
/**
 * 
 */
UCLASS()
class LEYR_API UBaseGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, Category="Input", meta=(Categories="InputTag"))
	FGameplayTag StartupInputTag;
	
	void MakeAndApplyExecuteEffectToTarget(const FGameplayTag& TagToApply, UAbilitySystemComponent* TargetASC, int32 Level = 1);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	UObject* GetSourceObjectFromAbilitySpec();

	UFUNCTION(BlueprintPure)
	ESequenceType GetSequenceType() const { return SequenceType; }
	
	UFUNCTION(BlueprintCallable)
	void SetGCDImpactSound(UObject* Object) const;
	
	UFUNCTION(BlueprintPure)
	TSoftObjectPtr<USoundBase> GetImpactSoundAsset() const;
	
	UFUNCTION(BlueprintCallable)
	bool CommitInventoryCost(bool bIsSelfCost = true);
	
	virtual FString GetDescription(int32 Level);
	virtual FString GetNextLevelDescription(int32 Level);
	static FString GetLockedDescription(int32 Level);

	UGameplayCueDefinition* GetGameplayCueDefinition() const { return GameplayCueDefinition; }
	void SetAbilityData(UAbilityData* Data);

protected:
	UFUNCTION(BlueprintCallable)
	virtual void InitAbility();
	
	UFUNCTION(BlueprintCallable)
	virtual void PrepareToEndAbility();
	
	float GetManaCost(float InLevel = 1.f) const;
	float GetCooldown(float InLevel = 1.f) const;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(Categories="Montage"), Category="Ability")
	FGameplayTag MontageTag = FGameplayTag();

	UPROPERTY(BlueprintReadWrite)
	FVector CursorHitLocation;
	
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UPaperZDAnimInstance> PaperAnimInstance = nullptr;
	
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UPaperZDAnimInstance> WeaponAnimInstance = nullptr;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UPaperZDAnimSequence> SelectedMontage = nullptr;
	
	UPROPERTY(BlueprintReadOnly)
	FVector HitImpulse = FVector::ZeroVector;

	FTaggedMontage TaggedMontage;
	FTaggedMontage WeaponTaggedMontage;
	TObjectPtr<UItemData> AbilityItemData = nullptr;
	TObjectPtr<UGameplayCueDefinition> GameplayCueDefinition;

	UPROPERTY(EditDefaultsOnly, Category="Ability")
	TObjectPtr<UAbilityData> AbilityData;

	/*
	 * Base Ability Data
	 */
	FGameplayTag DamageType;
	ESequenceType SequenceType = ESequenceType::Default;
	FScalableFloat AbilityPower;
	float AbilityPoise = 0.f;

private:
	FActiveGameplayEffectHandle ActiveSourceObjectEffectHandle;
	bool bPoiseWasApplied = false;
};
