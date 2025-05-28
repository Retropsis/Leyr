// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "PaperZD/Public/PaperZDAnimationComponent.h"
#include "AbilitySystemInterface.h"
#include "Data/AbilitySet.h"
#include "Interaction/CombatInterface.h"
#include "Leyr/Leyr.h"
#include "BaseCharacter.generated.h"

class UAbilityData;
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
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override { return AbilitySystemComponent; } 
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }
	UPaperZDAnimInstance* SetWeaponAnimInstance(const TSubclassOf<UPaperZDAnimInstance>& AnimInstance) const;

	UFUNCTION(BlueprintCallable)
	void ChangeDirections();
	
	UFUNCTION(BlueprintCallable)
	void HitStop(float Duration, float Amount = 0.f); 
	
	FOnDeath OnDeath;
	FOnASCRegistered OnASCRegistered;

protected:
	/*
	 * Ability System
	 */
	virtual void InitAbilityActorInfo() {}
	virtual void AddCharacterAbilities() {}
	virtual void InitializeDefaultAttributes() const {}
	virtual void InitializeCharacterInfo() {}
	virtual void HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount) {}
	virtual void HitReactFlashTagChanged(const FGameplayTag CallbackTag, int32 NewCount);
	virtual void StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount);
	virtual void BurnTagChanged(const FGameplayTag CallbackTag, int32 NewCount);
	void ApplyEffectToSelf(const TSubclassOf<UGameplayEffect>& GameplayEffectClass, float Level) const;
	void MakeAndApplyEffectToSelf(const FGameplayTag Tag, float Level = 1.f) const;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

	FAbilitySet_GrantedHandles GrantedHandles;

	/*
	 * Attacking
	 */
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> WeaponSocket;
	
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
	
	TEnumAsByte<EObjectTypeQuery> TraceObjectType = ObjectTypeQuery3;

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

	/*
	 * Status Effects
	 */
	UPROPERTY(BlueprintReadOnly, Category="Character|Combat")
	bool bHitReacting = false;
	
	UPROPERTY(ReplicatedUsing=OnRep_Burned, BlueprintReadOnly, Category="Character|Combat")
	bool bIsBurned = false;
	
	UPROPERTY(ReplicatedUsing=OnRep_Stunned, BlueprintReadOnly, Category="Character|Combat")
	bool bIsStunned = false;
	
	UPROPERTY(Replicated, BlueprintReadOnly)
	bool bIsElectrocuted = false;
	
	UFUNCTION()
	virtual void OnRep_Stunned() {}
 
	UFUNCTION()
	virtual void OnRep_Burned() {}

	//~ Combat Interface
	virtual UAbilityData* LoadAndGetDefaultAbilityData_Implementation() override;

	virtual TSoftObjectPtr<UPaperZDAnimSequence> GetHitReactSequence_Implementation() override { return HitReactSequence; }
	virtual USoundBase* ImpactSoundFromTag_Implementation(const FGameplayTag& MontageTag, ESequenceType SequenceType) override;
	virtual UNiagaraSystem* GetImpactEffect_Implementation() override { return ImpactEffect; }
	virtual void SetImpactSoundLoaded_Implementation(USoundBase* ImpactSound) override { ImpactSoundLoaded = ImpactSound; }
	virtual USoundBase* GetImpactSoundLoaded_Implementation() override { return ImpactSoundLoaded; }

	virtual void SetIsElectrocuted_Implementation(bool InIsElectrocuted) override { bIsElectrocuted = InIsElectrocuted; }
	virtual bool IsElectrocuted_Implementation() const override { return bIsElectrocuted; }
	
	virtual FVector GetCombatSocketLocation_Implementation(const FGameplayTag& MontageTag) override;
	virtual FVector GetRelativeCombatSocketLocation_Implementation(const FGameplayTag& MontageTag) override;
	virtual UPaperZDAnimInstance* GetPaperAnimInstance_Implementation() override { return AnimationComponent->GetAnimInstance(); }
	virtual UPaperZDAnimInstance* GetUpperBodyAnimInstance_Implementation() override { return UpperBodyComponent->GetAnimInstance(); }
	virtual UPaperZDAnimInstance* GetWeaponAnimInstance_Implementation() override { return WeaponComponent->GetAnimInstance(); }
	
	virtual void GetAttackAnimationData_Implementation(FVector& InBoxTraceStart, FVector& InBoxTraceEnd) override;
	virtual FBoxTraceData GetBoxTraceDataByTag_Implementation(FGameplayTag MontageTag, ESequenceType SequenceType) override;
	virtual TArray<FTaggedMontage> GetAttackMontages_Implementation(ESequenceType SequenceType) override;
	virtual FTaggedMontage GetTaggedMontageByTag_Implementation(const FGameplayTag& MontageTag, ESequenceType SequenceType) override;
	
	virtual int32 GetMinionCount_Implementation() override { return MinionCount; }
	virtual void IncrementMinionCount_Implementation(const int32 Amount) override { MinionCount += Amount; }
	virtual EObjectTypeQuery GetTraceObjectType_Implementation() override { return EOT_EnemyCapsule; }
	virtual ECharacterClass GetCharacterClass_Implementation() override { return CharacterClass; }
	virtual AActor* GetAvatar_Implementation() override { return this; }
	
	virtual FOnASCRegistered& GetOnASCRegistered() override { return OnASCRegistered; }
	virtual FOnDeath& GetOnDeath() override { return OnDeath; }
	virtual void BroadcastOnDeath_Implementation() override { OnDeath.Broadcast(this); }
	
	virtual void Die(const FVector& DeathImpulse, bool bExecute) override;
	void HandleDeathCapsuleComponent(const FVector& DeathImpulse);
	void HandleDeath(EDefeatState InDefeatState);
	virtual EDefeatState GetDefeatState_Implementation() const override { return  DefeatState; }
	virtual  bool IsDefeated_Implementation() const override { return DefeatState != EDefeatState::None; }
	virtual void SetGravityScale_Implementation(float GravityValue) override;
	virtual float GetGravityScale_Implementation() override;
	virtual void ResetGravityScale_Implementation() override;
	virtual bool IsCharacterAirborne_Implementation() override;
	virtual void SetMovementMode_Implementation(EMovementMode MovementMode,  float NewWalkingSpeed = -1.f, float GravityValue = -1.f) override;
	virtual void AdjustDirectionToFaceTarget_Implementation(AActor* Target) override;
	virtual void AddImpulse_Implementation(FVector Impulse) override;
	//~ Combat Interface
	
	FTaggedMontage GetTaggedMontageInfoByTag(const FGameplayTag& MontageTag, ESequenceType SequenceType) const;

	UFUNCTION(NetMulticast, Reliable)
	virtual void MulticastHandleDeath(const FVector& DeathImpulse, EDefeatState InDefeatState);
	
	UPROPERTY(BlueprintReadOnly)
	EDefeatState DefeatState = EDefeatState::None;

	/*
	 * Animation
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="PaperZD")
	TObjectPtr<UPaperFlipbookComponent> UpperBody;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="PaperZD")
	TObjectPtr<UPaperFlipbookComponent> WeaponFlipbook;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="PaperZD")
	TObjectPtr<UPaperZDAnimationComponent> AnimationComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="PaperZD")
	TObjectPtr<UPaperZDAnimationComponent> UpperBodyComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="PaperZD")
	TObjectPtr<UPaperZDAnimationComponent> WeaponComponent;

	/*
	 * Data Asset
	 */
	UPROPERTY() TObjectPtr<UNiagaraSystem> ImpactEffect;
	UPROPERTY() TObjectPtr<USoundBase> ImpactSoundLoaded;
	UPROPERTY() TObjectPtr<USoundBase> DefeatedSoundLoaded;
	UPROPERTY() TSoftObjectPtr<UPaperZDAnimSequence> HitReactSequence;

	UPROPERTY(EditAnywhere, Category="Character|Combat")
	TObjectPtr<UAttackSequenceInfo> AttackSequenceInfo;
	
	UPROPERTY(EditAnywhere, Category="Character|Combat")
	TSoftObjectPtr<UAbilityData> DefaultAbilityData;
	
	/* Minions */
	int32 MinionCount = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character|Combat")
	ECharacterClass CharacterClass = ECharacterClass::Warrior;

	/*
	 * Visual Effects
	 */ 
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStatusEffectNiagaraComponent> BurnStatusEffectComponent;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStatusEffectNiagaraComponent> StunStatusEffectComponent;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr< UMaterialInstanceDynamic> DynamicSpriteInstance = nullptr;

	UPROPERTY(EditDefaultsOnly, Category="Character|HitReact")
	float HitReactFlashStrength = 1.f;
	
	UPROPERTY(EditDefaultsOnly, Category="Character|HitReact")
	float HitReactFlashPlayRate = 1.f;
	
	UPROPERTY(EditDefaultsOnly, Category="Character|HitReact")
	FLinearColor HitReactFlashColor = FLinearColor::Red;

	UFUNCTION(BlueprintImplementableEvent)
	void PlayFlashEffect(float Strength, float PlayRate, FLinearColor Color);
};
