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

USTRUCT(BlueprintType)
struct FInitAbilityParams
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	bool bResetPitch = true;

	UPROPERTY(BlueprintReadWrite)
	FGameplayTagContainer TagsToApply;
};

USTRUCT(BlueprintType)
struct FEndAbilityParams
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	bool bResetPitch = true;
	
	UPROPERTY(BlueprintReadWrite)
	FGameplayTagContainer TagsToRemove;
};

/**
 * EAbilityActivationPolicy
 *
 *	Defines how an ability is meant to activate.
 */
UENUM(BlueprintType)
enum class EAbilityActivationPolicy : uint8
{
	// Try to activate the ability when the input is triggered.
	OnInputTriggered,

	// Continually try to activate the ability while the input is active.
	WhileInputActive,

	// Try to activate the ability when an avatar is assigned.
	OnSpawn
};

/**
 * 
 */
UCLASS()
class LEYR_API UBaseGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	
	void MakeAndApplyExecuteEffectToTarget(const FGameplayTag& TagToApply, UAbilitySystemComponent* TargetASC, int32 Level = 1);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	UObject* GetSourceObjectFromAbilitySpec();

	UFUNCTION(BlueprintPure)
	ESequenceType GetSequenceType() const { return SequenceType; }
	
	UFUNCTION(BlueprintCallable)
	void SetOwningCharacterCapsuleCollision(ECollisionChannel Channel, ECollisionResponse Response) const;
	
	UFUNCTION(BlueprintCallable)
	void SetGCDImpactSound(UObject* Object) const;
	
	UFUNCTION(BlueprintPure)
	TSoftObjectPtr<USoundBase> GetImpactSoundAsset() const;
	
	UFUNCTION(BlueprintCallable)
	bool CommitInventoryCost(bool bIsSelfCost = true);
	
	UFUNCTION(BlueprintCallable, meta=(GameplayTagFilter="GameplayCue"))
	void ExecuteAbilityGameplayCue(FGameplayTag GameplayCueTag, const FGameplayTag AdditionalTag);
	
	virtual FString GetDescription(int32 Level);
	virtual FString GetNextLevelDescription(int32 Level);
	static FString GetLockedDescription(int32 Level);

	UGameplayCueDefinition* GetGameplayCueDefinition() const { return GameplayCueDefinition; }
	void SetAbilityData(UAbilityData* Data);
	
	UPROPERTY(EditDefaultsOnly, Category="Ability|Input", meta=(Categories="InputTag"))
	FGameplayTag StartupInputTag;
	
	UPROPERTY(EditDefaultsOnly, Category="Ability|Assets")
	TObjectPtr<USoundBase>  AbilitySound = nullptr;
	
	UPROPERTY(EditDefaultsOnly, Category="Ability|Assets")
	TObjectPtr<UNiagaraSystem>  AbilityEffect = nullptr;

protected:
	void TryActivateAbilityOnSpawn(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) const;
	bool FetchAnimInstances();
	void InitializeDefaultAbilityData();
	void InitializeItemAbilityData();
	void DeterminePoise();

	UFUNCTION(BlueprintCallable)
	virtual void InitAbility();
	
	UFUNCTION(BlueprintCallable)
	virtual void InitAbilityWithParams(FInitAbilityParams Params);
	void RemovePoise() const;

	UFUNCTION(BlueprintCallable)
	virtual void PrepareToEndAbility();
	
	UFUNCTION(BlueprintCallable)
	virtual void PrepareToEndAbilityWithParams(FEndAbilityParams Params);
	
	UFUNCTION(BlueprintCallable)
	virtual void SetCurrentSequence();
	
	float GetManaCost(float InLevel = 1.f) const;
	float GetCooldown(float InLevel = 1.f) const;

	// Defines how this ability is meant to activate.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Ability|Ability Activation")
	EAbilityActivationPolicy ActivationPolicy = EAbilityActivationPolicy::OnInputTriggered;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(Categories="Montage"), Category="Ability")
	FGameplayTag MontageTag = FGameplayTag();

	UPROPERTY(BlueprintReadWrite)
	FVector CursorHitLocation;
	
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UPaperZDAnimInstance> PaperAnimInstance = nullptr;
	
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UPaperZDAnimInstance> UpperBodyAnimInstance = nullptr;
	
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
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Ability")
	float MinAbilityTime = 0.5f;
	
	/*
	 * Base Ability Data
	 */
	FGameplayTag DamageType;
	FGameplayTag StatusEffectType;
	ESequenceType SequenceType = ESequenceType::Default;
	FScalableFloat AbilityPower;
	float AbilityPoise = 0.f;
	float AbilityLevelRatio = 1.f;
	float CharacterLevelRatio = 1.f;

private:
	FActiveGameplayEffectHandle ActiveSourceObjectEffectHandle;
	FActiveGameplayEffectHandle ActivePoiseEffectHandle;
	bool bResetPitch = true;
};
