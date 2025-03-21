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
	
	UPROPERTY(EditDefaultsOnly, Category="Ability|Damage")
	bool bShouldApplyExecute = false;
	
	UPROPERTY(EditDefaultsOnly, Category="Ability|Damage")
	bool bShouldExecute = false;

	/*
	 * Status Effect
	 */
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