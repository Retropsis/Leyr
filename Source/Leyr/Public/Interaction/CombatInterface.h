// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Leyr/Leyr.h"
#include "UObject/Interface.h"
#include "CombatInterface.generated.h"

class UNiagaraSystem;
class UPaperZDAnimSequence;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnASCRegistered, UAbilitySystemComponent*);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDeath, AActor*, DeadActor);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnTakeDamage, float /* DamageAmount */);

USTRUCT(BlueprintType)
struct FTaggedMontage
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FValueRange AbilityPower = FValueRange();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UPaperZDAnimSequence* Montage = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag MontageTag;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag SocketTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USoundBase* ImpactSound = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FVector BoxTraceStart = FVector::ZeroVector;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FVector BoxTraceEnd = FVector::ZeroVector;
};

UENUM(BlueprintType)
enum class ECombatState : uint8
{
	Unoccupied UMETA(DisplayName="Unoccupied"),
	Attacking UMETA(DisplayName="Attacking"),
};

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
	UFUNCTION(BlueprintNativeEvent)
	int32 GetCharacterLevel();
	
	virtual void Die() = 0;
	// virtual FOnASCRegistered& GetOnASCRegistered() = 0;
	// virtual FOnDeath& GetOnDeathDelegate() = 0;
	// virtual FOnTakeDamage& GetOnTakeDamage() = 0;
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FVector GetCombatSocketLocation(const FGameplayTag& MontageTag);
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void UpdateFacingTarget(const FVector& Target);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UAnimMontage* GetHitReactMontage();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UPaperZDAnimSequence* GetHitReactSequence();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UPaperZDAnimSequence* GetAttackSequence();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UPaperZDAnimInstance* GetPaperAnimInstance();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void GetAttackAnimationData(FVector& InBoxTraceStart, FVector& InBoxTraceEnd);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FTaggedMontage GetTaggedMontage();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	TArray<FTaggedMontage> GetAttackMontages();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FTaggedMontage GetTaggedMontageByTag(const FGameplayTag& MontageTag);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UNiagaraSystem* GetImpactEffect();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	int32 GetMinionCount();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void IncrementMinionCount(int32 Amount);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	EObjectTypeQuery GetTraceObjectType();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool IsDead() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	AActor* GetAvatar();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	ECharacterClass GetCharacterClass();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	ECombatState GetCombatState() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetCombatState(ECombatState NewState);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetMovementEnabled(bool Enabled);

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
};
