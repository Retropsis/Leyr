// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "PaperZD/Public/PaperZDAnimationComponent.h"
#include "AbilitySystemInterface.h"
#include "Interaction/CombatInterface.h"
#include "Leyr/Leyr.h"
#include "BaseCharacter.generated.h"

class UGameplayAbility;
class UGameplayEffect;
class UAttributeSet;
class UAbilitySystemComponent;

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

	//~ Combat Interface
	virtual FVector GetCombatSocketLocation_Implementation() override;
	virtual UAnimMontage* GetHitReactMontage_Implementation() override { return HitReactMontage; }
	virtual UPaperZDAnimSequence* GetHitReactSequence_Implementation() override { return HitReactSequence; }
	virtual UPaperZDAnimSequence* GetAttackSequence_Implementation() override { return AttackSequence; }
	virtual UPaperZDAnimInstance* GetPaperAnimInstance_Implementation() override { return AnimationComponent->GetAnimInstance(); }
	virtual void GetAttackAnimationData_Implementation(FVector& InBoxTraceStart, FVector& InBoxTraceEnd) override;
	virtual EObjectTypeQuery GetTraceObjectType_Implementation() override { return EOT_EnemyCapsule; }
	virtual void Die() override;
	virtual bool IsDead_Implementation() const override { return  bDead; }
	virtual AActor* GetAvatar_Implementation() override { return this; }
	//~ Combat Interface

	UFUNCTION(NetMulticast, Reliable)
	virtual void MulticastHandleDeath();

	UPROPERTY(BlueprintReadOnly)
	bool bDead = false;
	
	UPROPERTY(EditAnywhere, Category="Character|Combat")
	TObjectPtr<UPaperZDAnimSequence> AttackSequence;
	
	UPROPERTY(EditAnywhere, Category="Character|Combat")
	TObjectPtr<UAnimMontage> HitReactMontage;
	
	UPROPERTY(EditAnywhere, Category="Character|Combat")
	TObjectPtr<UPaperZDAnimSequence> HitReactSequence;

	UPROPERTY(EditAnywhere, Category="Character|Combat")
	TEnumAsByte<EObjectTypeQuery> TraceObjectType = ObjectTypeQuery3;
	
	/*
	 * Dissolve Effects
	 */ 
	void Dissolve();

	UFUNCTION(BlueprintImplementableEvent)
	void StartDissolveTimeline(UMaterialInstanceDynamic* DynamicMaterialInstance);

	UFUNCTION(BlueprintImplementableEvent)
	void StartWeaponDissolveTimeline(UMaterialInstanceDynamic* DynamicMaterialInstance);

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UMaterialInstance> DissolveMaterialInstance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UMaterialInstance> WeaponDissolveMaterialInstance;
	
private:
	UPROPERTY(EditAnywhere, Category="Character|Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="PaperZD", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UPaperZDAnimationComponent> AnimationComponent;
};
