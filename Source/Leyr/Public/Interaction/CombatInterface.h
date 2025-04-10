// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Data/CombatData.h"
#include "UObject/Interface.h"
#include "CombatInterface.generated.h"

enum class ESequenceType : uint8;
class UNiagaraSystem;
class UPaperZDAnimSequence;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnASCRegistered, UAbilitySystemComponent*);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDeath, AActor*, DeadActor);

// This class does not need to be modified.
UINTERFACE(MinimalAPI, BlueprintType)
class UCombatInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class LEYR_API ICombatInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:	
	virtual void Die(const FVector& DeathImpulse, bool bExecute) = 0;
	virtual FOnASCRegistered& GetOnASCRegistered() = 0;
	virtual FOnDeath& GetOnDeath() = 0;
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void UpdateFacingTarget(const FVector& Target);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void AdjustDirectionToFaceTarget(AActor* Target);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UAbilityData* LoadAndGetDefaultAbilityData();

	/*
	 * Getter - Setter
	*/
	UFUNCTION(BlueprintNativeEvent)
	int32 GetCharacterLevel();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FVector GetCombatSocketLocation(const FGameplayTag& MontageTag);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FVector GetRelativeCombatSocketLocation(const FGameplayTag& MontageTag);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	TSoftObjectPtr<UPaperZDAnimSequence> GetHitReactSequence();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UNiagaraSystem* GetImpactEffect();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	TSoftObjectPtr<USoundBase> ImpactSoundFromTag(const FGameplayTag& MontageTag, ESequenceType SequenceType);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetImpactSoundLoaded(USoundBase* ImpactSound);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	USoundBase* GetImpactSoundLoaded();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UPaperZDAnimInstance* GetPaperAnimInstance();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UPaperZDAnimInstance* GetUpperBodyAnimInstance();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UPaperZDAnimInstance* GetWeaponAnimInstance();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void GetAttackAnimationData(FVector& InBoxTraceStart, FVector& InBoxTraceEnd);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FBoxTraceData GetBoxTraceDataByTag(FGameplayTag MontageTag, ESequenceType SequenceType);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FTaggedMontage GetTaggedMontage();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	TArray<FTaggedMontage> GetAttackMontages(ESequenceType SequenceType);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FTaggedMontage GetTaggedMontageByTag(const FGameplayTag& MontageTag, ESequenceType SequenceType);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FTaggedMontage GetTaggedMontageByIndex(int32 Index);

	/*
	 * Spells
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	int32 GetMinionCount();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void IncrementMinionCount(int32 Amount);

	/*
	 * Status Effects
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool IsElectrocuted() const;
 
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetIsElectrocuted(bool InIsElectrocuted);

	/*
	 *
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	EObjectTypeQuery GetTraceObjectType();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	EDefeatState GetDefeatState() const;
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool IsDefeated() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	AActor* GetAvatar();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	ECharacterClass GetCharacterClass();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	ECombatState GetCombatState() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetCombatState(ECombatState NewState);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetCombatStateToHandle(ECombatState NewState);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetMovementEnabled(bool Enabled);

	/*
	 *
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetComboWindow(bool bOpen);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool IsComboWindowOpen();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ResetAttacking();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ResetCombatVariables();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	int32 GetAttackComboIndex();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetAttackComboIndex(int32 Index);

	/*
	 *
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetGravityScale(float GravityValue);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	float GetGravityScale(); 

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ResetGravityScale();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetMovementMode(EMovementMode MovementMode,  float NewWalkingSpeed, float GravityValue);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool IsCharacterAirborne();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetMovementTarget(FVector Target);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetSpriteRelativeLocation(FVector NewLocation);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	float GetOverridePitch();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void AddImpulse(FVector Impulse);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ResetAimingPitch();
};
