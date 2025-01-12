// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "PaperZD/Public/PaperZDAnimationComponent.h"
#include "AbilitySystemInterface.h"
#include "Interaction/CombatInterface.h"
#include "Leyr/Leyr.h"
#include "BaseCharacter.generated.h"

class UAttackSequenceInfo;
class UGameplayAbility;
class UGameplayEffect;
class UAttributeSet;
class UAbilitySystemComponent;
class UStatusEffectNiagaraComponent;

/**
 * 
 */
UCLASS()
class LEYR_API ABaseCharacter : public APaperCharacter, public IAbilitySystemInterface, public ICombatInterface
{
	GENERATED_BODY()

public:
	ABaseCharacter();
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }
	
	void ChangeDirections();

protected:
	/*
	 * Ability System
	 */
	virtual void InitAbilityActorInfo() {}
	virtual void InitializeDefaultAttributes() const;
	void ApplyEffectToSelf(const TSubclassOf<UGameplayEffect>& GameplayEffectClass, float Level) const;
	void AddCharacterAbilities() const;
	virtual void HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount) {}
	void MakeAndApplyEffectToSelf(const FGameplayTag Tag, float Level = 1.f) const;
	
	FOnASCRegistered OnASCRegistered;
	FOnDeath OnDeath;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Character|Attributes")
	TSubclassOf<UGameplayEffect> DefaultPrimaryAttributes;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Character|Attributes")
	TSubclassOf<UGameplayEffect> DefaultSecondaryAttributes;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Character|Attributes")
	TSubclassOf<UGameplayEffect> DefaultVitalAttributes;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

	/*
	 * Attacking
	 */
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> Weapon;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USceneComponent> BoxTraceStart;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USceneComponent> BoxTraceEnd;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FVector BoxTraceExtent = FVector(20.f);
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FRotator BoxTraceRotation = FRotator::ZeroRotator;
	
	UPROPERTY(EditAnywhere, Category="Character|Combat")
	FName WeaponTipSocketName;
	
	UPROPERTY(EditAnywhere, Category="Character|Combat")
	FName WeaponSocketName;
	
	TEnumAsByte<EObjectTypeQuery> TraceObjectType = EObjectTypeQuery::ObjectTypeQuery3;

	/*
	 * Movement
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Character|Movement")
	float BaseGravityScale = 3.25f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character|Movement")
	float BaseWalkSpeed = 120.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character|Movement")
	float BaseWalkSpeedCrouched = 120.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character|Movement")
	float BaseFlySpeed = 300.f;

	UPROPERTY(BlueprintReadOnly, Category="Character|Combat")
	bool bHitReacting = false;

	//~ Combat Interface
	virtual FVector GetCombatSocketLocation_Implementation(const FGameplayTag& MontageTag) override;
	virtual UPaperZDAnimSequence* GetHitReactSequence_Implementation() override { return HitReactSequence; }
	virtual UPaperZDAnimInstance* GetPaperAnimInstance_Implementation() override { return AnimationComponent->GetAnimInstance(); }
	virtual void GetAttackAnimationData_Implementation(FVector& InBoxTraceStart, FVector& InBoxTraceEnd) override;
	virtual FBoxTraceData GetBoxTraceDataByTag_Implementation(FGameplayTag MontageTag, ESequenceType SequenceType) override;
	virtual TArray<FTaggedMontage> GetAttackMontages_Implementation(ESequenceType SequenceType) override;
	virtual FTaggedMontage GetTaggedMontageByTag_Implementation(const FGameplayTag& MontageTag, ESequenceType SequenceType) override;
	virtual UNiagaraSystem* GetImpactEffect_Implementation() override { return ImpactEffect; }
	virtual int32 GetMinionCount_Implementation() override { return MinionCount; }
	virtual void IncrementMinionCount_Implementation(const int32 Amount) override { MinionCount += Amount; }
	virtual EObjectTypeQuery GetTraceObjectType_Implementation() override { return EOT_EnemyCapsule; }
	virtual ECharacterClass GetCharacterClass_Implementation() override { return CharacterClass; }
	virtual AActor* GetAvatar_Implementation() override { return this; }
	virtual FOnASCRegistered& GetOnASCRegistered() override { return OnASCRegistered; }
	virtual FOnDeath& GetOnDeath() override { return OnDeath; }
	virtual void Die(const FVector& DeathImpulse) override;
	virtual bool IsDead_Implementation() const override { return  bDead; }
	virtual void SetGravityScale_Implementation(float GravityValue) override;
	virtual float GetGravityScale_Implementation() override;
	virtual void ResetGravityScale_Implementation() override;
	virtual bool IsCharacterAirborne_Implementation() override;
	virtual void SetMovementMode_Implementation(EMovementMode MovementMode,  float NewWalkingSpeed = -1.f, float GravityValue = -1.f) override;
	virtual void AdjustDirection_Implementation() override;
	virtual void AddImpulse_Implementation(FVector Impulse) override;
	//~ Combat Interface
	
	FTaggedMontage GetTaggedMontageInfoByTag(const FGameplayTag& MontageTag, ESequenceType SequenceType) const;

	UFUNCTION(NetMulticast, Reliable)
	virtual void MulticastHandleDeath(const FVector& DeathImpulse);

	UPROPERTY(BlueprintReadOnly)
	bool bDead = false;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="PaperZD")
	TObjectPtr<UPaperFlipbookComponent> UpperBody;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="PaperZD")
	TObjectPtr<UPaperZDAnimationComponent> AnimationComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="PaperZD")
	TObjectPtr<UPaperZDAnimationComponent> UpperBodyAnimationComponent;

	/*
	 * Data Asset
	 */
	virtual void InitializeCharacterInfo() {}

	UPROPERTY() TObjectPtr<UNiagaraSystem> ImpactEffect;
	UPROPERTY() TObjectPtr<USoundBase> DeathSound;
	
	UPROPERTY(EditAnywhere, Category="Character|Combat")
	TObjectPtr<UPaperZDAnimSequence> HitReactSequence;

	UPROPERTY(EditAnywhere, Category="Character|Combat")
	TObjectPtr<UAttackSequenceInfo> AttackSequenceInfo;
	
	/* Minions */
	int32 MinionCount = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character Class Defaults")
	ECharacterClass CharacterClass = ECharacterClass::Warrior;

	/*
	 * Visual Effects
	 */ 
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStatusEffectNiagaraComponent> BurnStatusEffectComponent;
	
private:	
	UPROPERTY(EditAnywhere, Category="Character|Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;
	
	UPROPERTY(EditAnywhere, Category = "Character|Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupPassiveAbilities;
	
	UPROPERTY(EditAnywhere, Category = "Character|Abilities")
	TArray<TSubclassOf<UGameplayAbility>> CommonAbilities;
};
