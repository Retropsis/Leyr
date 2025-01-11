// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "AbilityTypes.h"
#include "AbilitySystem/Ability/BaseGameplayAbility.h"
#include "AbilitySystem/Data/AttackSequenceInfo.h"
#include "Interaction/CombatInterface.h"
#include "DamageGameplayAbility.generated.h"

class UPaperZDAnimInstance;
/**
 * 
 */
UCLASS()
class LEYR_API UDamageGameplayAbility : public UBaseGameplayAbility
{
	GENERATED_BODY()
	
public:
	virtual void InitAbility() override;
	virtual void PrepareToEndAbility() override;
	
	UFUNCTION(BlueprintCallable)
	TArray<FHitResult> BoxTrace(bool bDebug = false);
	
	UFUNCTION(BlueprintCallable)
	void CauseDamage(UAbilitySystemComponent* TargetASC);

	UFUNCTION(BlueprintCallable)
	void ForEachHitTryCausingDamage(TArray<FHitResult> HitResults);

	void MakeAndApplyExecuteEffectToTarget(const FGameplayTag& TagToApply, UAbilitySystemComponent* TargetASC, int32 Level = 1);

	UFUNCTION(BlueprintPure)
	bool IsHostile() const;

	UFUNCTION(BlueprintCallable)
	void SetCurrentComboSequence();
	
	UFUNCTION(BlueprintCallable)
	void SetCurrentSequence();
	
	UFUNCTION(BlueprintCallable)
	void SelectMontageTagFromCombatState();
	
	UFUNCTION(BlueprintPure)
	FAdditionalEffectParams MakeAdditionalEffectParamsFromClassDefaults(AActor* TargetActor = nullptr) const;
	
protected:
	UPROPERTY(EditDefaultsOnly, Category="Ability|Defaults")
	bool bShouldApplyExecute = false;
	
	UPROPERTY(BlueprintReadWrite, Category="Ability|Defaults")
	bool bHasHitTarget = false;

	UPROPERTY(BlueprintReadWrite, Category="Ability|Defaults")
	TObjectPtr<AActor> HitActor = nullptr;
	
	UPROPERTY(BlueprintReadOnly, Category="Ability|Defaults")
	TObjectPtr<UPaperZDAnimInstance> PaperAnimInstance = nullptr;

	UPROPERTY(BlueprintReadOnly, Category="Ability|Defaults")
	TObjectPtr<UPaperZDAnimSequence> SelectedMontage = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(Categories="Montage"), Category="Ability|Defaults")
	FGameplayTag MontageTag = FGameplayTag();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Ability|Defaults")
	ESequenceType SequenceType = ESequenceType::Default;

	/*
	 * Damage
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Ability|Damage")
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	UPROPERTY(EditDefaultsOnly, Category="Ability|Damage", meta=(Categories="Damage"))
	FGameplayTag DamageType;

	UPROPERTY(EditDefaultsOnly, Category="Ability|Status Effect")
	float StatusEffectChance = 20.f;

	UPROPERTY(EditDefaultsOnly, Category="Ability|Status Effect")
	float StatusEffectDamage = 5.f;

	UPROPERTY(EditDefaultsOnly, Category="Ability|Status Effect")
	float StatusEffectFrequency = 1.f;

	UPROPERTY(EditDefaultsOnly, Category="Ability|Status Effect")
	float StatusEffectDuration = 5.f;
	
	UPROPERTY(EditDefaultsOnly, Category="Ability|Status Effect")
	float DeathImpulseMagnitude = 250.f;

	UPROPERTY(EditDefaultsOnly, Category="Ability|Status Effect")
	float AirborneForceMagnitude = 250.f;

	UPROPERTY(EditDefaultsOnly, Category="Ability|Status Effect")
	float AirborneChance = 0.f;

	UFUNCTION(BlueprintPure)
	FTaggedMontage GetRandomTaggedMontageFromArray(const TArray<FTaggedMontage>& TaggedMontages) const;
	
	// FVector2D GetDamageByDamageType(float InLevel, const FGameplayTag& DamageType);

private:	
	FBoxTraceData BoxTraceData = FBoxTraceData();
};