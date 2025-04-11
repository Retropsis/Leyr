// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "AbilityTypes.h"
#include "AbilitySystem/Ability/BaseGameplayAbility.h"
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
	
	UFUNCTION(BlueprintPure)
	float GetAbilityPowerAtLevel() const;
	
	UFUNCTION(BlueprintPure)
	float GetRadialDamageInnerRadius() const { return RadialDamageInnerRadius; }
	
	UFUNCTION(BlueprintPure)
	float GetRadialDamageOuterRadius() const { return RadialDamageOuterRadius; }
	
	UFUNCTION(BlueprintCallable)
	void ApplyHitStop();

	UFUNCTION(BlueprintPure)
	bool IsHostile() const;

	UFUNCTION(BlueprintCallable)
	void SetCurrentComboSequence();
	
	UFUNCTION(BlueprintCallable)
	void SetCurrentSequence();
	
	UFUNCTION(BlueprintCallable)
	void SelectMontageTagFromCombatState();
	
	UFUNCTION(BlueprintCallable, meta=(GameplayTagFilter="GameplayCue"))
	void ExecuteDamageGameplayCue(FGameplayTag GameplayCueTag);
	
	UFUNCTION(BlueprintPure)
	FAdditionalEffectParams MakeAdditionalEffectParamsFromClassDefaults(AActor* TargetActor = nullptr) const;
	
	UFUNCTION(BlueprintPure)
	FAdditionalEffectParams MakeStatusEffectParamsFromClassDefaults(AActor* TargetActor = nullptr) const;
	
protected:		
	UPROPERTY(BlueprintReadOnly)
	bool bHasHitTarget = false;

	UPROPERTY(BlueprintReadOnly)
	bool bExecuteSuccessful = false;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AActor> HitActor = nullptr;

	UPROPERTY()
	TArray<AActor*> HitActors;
	
	UPROPERTY(BlueprintReadOnly)
	FVector HitLocation = FVector::ZeroVector;
	
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UPaperZDAnimSequence> SelectedWeaponMontage = nullptr;

	/*
	 * Damage Ability Data
	 */
	TSubclassOf<UGameplayEffect> DamageEffectClass;
	TSubclassOf<UGameplayEffect> StatusEffectClass;
	
	UPROPERTY(EditDefaultsOnly, Category="Ability|Damage")
	bool bShouldApplyExecute = false;
	
	UPROPERTY(EditDefaultsOnly, Category="Ability|Damage")
	bool bShouldExecute = false;
	
	bool bIsRadialDamage = false;
	float RadialDamageInnerRadius = 0.f;
	float RadialDamageOuterRadius = 0.f;
	
	UPROPERTY(BlueprintReadWrite)
	FVector RadialDamageOrigin = FVector::ZeroVector;

	/*
	 * Status Effect Data
	 */
	float StatusEffectChance = 20.f;
	float StatusEffectDamage = 5.f;
	float StatusEffectFrequency = 1.f;
	float StatusEffectDuration = 5.f;
	float DeathImpulseMagnitude = 250.f;
	float AirborneForceMagnitude = 250.f;
	float AirborneChance = 0.f;

	UFUNCTION(BlueprintPure)
	FTaggedMontage GetRandomTaggedMontageFromArray(const TArray<FTaggedMontage>& TaggedMontages) const;
	
	// FVector2D GetDamageByDamageType(float InLevel, const FGameplayTag& DamageType);

private:	
	FBoxTraceData BoxTraceData = FBoxTraceData();
};