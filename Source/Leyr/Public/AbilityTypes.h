#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "AbilityTypes.generated.h"

class UGameplayEffect;

USTRUCT(BlueprintType)
struct FAdditionalEffectParams
{
	GENERATED_BODY()

	FAdditionalEffectParams(){}

	UPROPERTY() TObjectPtr<UObject> WorldContextObject = nullptr;
	UPROPERTY() TSubclassOf<UGameplayEffect> AdditionalEffectClass = nullptr;
	UPROPERTY() TObjectPtr<UAbilitySystemComponent> SourceAbilitySystemComponent;
	UPROPERTY() TObjectPtr<UAbilitySystemComponent> TargetAbilitySystemComponent;
	UPROPERTY() float AbilityPower = 0.f;
	UPROPERTY() float AbilityLevel = 1.f;
	UPROPERTY() FGameplayTag DamageType = FGameplayTag();
	UPROPERTY() float StatusEffectChance = 0.f;
	UPROPERTY() float StatusEffectDamage = 0.f;
	UPROPERTY() float StatusEffectDuration = 0.f;
	UPROPERTY() float StatusEffectFrequency = 0.f;
};

USTRUCT(BlueprintType)
struct FBaseGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()

public:
	bool IsCriticalHit() const { return bIsCriticalHit; }
	bool IsBlockedHit () const { return bIsBlockedHit; }
	bool IsSuccessfulStatusEffect() const { return bIsSuccessfulStatusEffect; }
	float GetStatusEffectDamage() const { return StatusEffectDamage; }
	float GetStatusEffectDuration() const { return StatusEffectDuration; }
	float GetStatusEffectFrequency() const { return StatusEffectFrequency; }
	TSharedPtr<FGameplayTag> GetDamageType() const { return DamageType; }

	void SetIsCriticalHit(bool bInIsCriticalHit) { bIsCriticalHit = bInIsCriticalHit; }
	void SetIsBlockedHit(bool bInIsBlockedHit) { bIsBlockedHit = bInIsBlockedHit; }
	void SetIsSuccessfulStatusEffect(bool bInIsStatusEffect) { bIsSuccessfulStatusEffect = bInIsStatusEffect; }
	void SetStatusEffectDamage(float InDamage) { StatusEffectDamage = InDamage; }
	void SetStatusEffectDuration(float InDuration) { StatusEffectDuration = InDuration; }
	void SetStatusEffectFrequency(float InFrequency) { StatusEffectFrequency = InFrequency; }
	void SetDamageType(const TSharedPtr<FGameplayTag>& InDamageType) { DamageType = InDamageType; }
	
	/** Returns the actual struct used for serialization, subclasses must override this! */
	virtual UScriptStruct* GetScriptStruct() const
	{
		return StaticStruct();
	}

	/** Creates a copy of this context, used to duplicate for later modifications */
	virtual FBaseGameplayEffectContext* Duplicate() const
	{
		FBaseGameplayEffectContext* NewContext = new FBaseGameplayEffectContext();
		*NewContext = *this;
		if (GetHitResult())
		{
			// Does a deep copy of the hit result
			NewContext->AddHitResult(*GetHitResult(), true);
		}
		return NewContext;
	}

	/** Custom serialization, subclasses must override this */
	virtual bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess);

protected:
	UPROPERTY()
	bool bIsBlockedHit = false;

	UPROPERTY()
	bool bIsCriticalHit = false;
	
	UPROPERTY()
	bool bIsSuccessfulStatusEffect = false;

	UPROPERTY()
	float StatusEffectDamage = 0.f;

	UPROPERTY()
	float StatusEffectDuration = 0.f;

	UPROPERTY()
	float StatusEffectFrequency = 0.f;

	TSharedPtr<FGameplayTag> DamageType;
};

template<>
struct TStructOpsTypeTraits<FBaseGameplayEffectContext> : public TStructOpsTypeTraitsBase2<FBaseGameplayEffectContext>
{
	enum
	{
		WithNetSerializer = true,
		WithCopy = true
	};
};