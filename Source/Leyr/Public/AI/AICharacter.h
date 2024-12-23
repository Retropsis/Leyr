// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "AI/BaseCharacter.h"
#include "AIData.h"
#include "AbilitySystem/Data/EncounterInfo.h"
#include "Interaction/AIInterface.h"
#include "Interaction/EnemyInterface.h"
#include "UI/Controller/OverlayWidgetController.h"
#include "AICharacter.generated.h"

enum class EEncounterName : uint8;
enum class EBehaviourType : uint8;
class ASplineComponentActor;
class ASplineMeshActor;
class USplineMeshComponent;
class USplineComponent;
class ABaseAIController;
class UBehaviorTree;
class UWidgetComponent;
/**
 * 
 */
UCLASS()
class LEYR_API AAICharacter : public ABaseCharacter, public IEnemyInterface, public IAIInterface
{
	GENERATED_BODY()

public:
	AAICharacter();
	virtual void Tick(float DeltaSeconds) override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount) override;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
	
	/** Combat Interface */
	virtual int32 GetCharacterLevel_Implementation() override { return Level; }
	virtual void SetCombatTarget_Implementation(AActor* InCombatTarget) override { CombatTarget = InCombatTarget; }
	virtual AActor* GetCombatTarget_Implementation() const override { return CombatTarget; }
	virtual void Die(const FVector& DeathImpulse) override;
	/** end Combat Interface */
	
	/** AI Interface */
	virtual FVector FindRandomLocation_Implementation() override;
	virtual bool MoveToLocation_Implementation(FVector TargetLocation, float Threshold) override;
	virtual bool ChaseTarget_Implementation(AActor* TargetToChase) override;
	virtual void SineMove_Implementation() override;
	virtual bool FollowSpline_Implementation(int32 SplineIndex) override;
	virtual FVector GetNextLocation_Implementation(int32 SplineIndex) override;
	/** end AI Interface */

	/** Enemy Interface */
	virtual void SetShouldAttack_Implementation(bool InShouldAttack) override { bShouldAttack = InShouldAttack; }
	virtual bool ShouldAttack_Implementation() const override { return bShouldAttack; }
	/** Enemy Interface */
	
	UPROPERTY(BlueprintReadWrite, Category = "Combat")
	TObjectPtr<AActor> CombatTarget;
	
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnMaxHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FOnGameplayTagAddedOrRemoved OnGameplayTagAddedOrRemoved;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character|Combat")
	float LifeSpan = 5.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Character|AI")
	float ChasingFlyingSpeed = 250.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character|AI")
	TObjectPtr<ASplineComponentActor> SplineComponentActor;
	
	TObjectPtr<USplineComponent> SplineComponent;

	UPROPERTY(EditAnywhere, Category = "Character|AI")
	EMovementType MovementType = EMovementType::Destination;

protected:
	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo() override;
	virtual void InitializeDefaultAttributes() const override;
	void InitializeBehaviourInfo();
	void AddAICharacterAbilities() const;
	
	UFUNCTION(BlueprintCallable)
	void CauseDamage(AActor* TargetActor);

	UFUNCTION(BlueprintCallable)
	void ResetShouldAttack();
	
	UFUNCTION()
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Encounter Defaults")
	EEncounterName EncounterName = EEncounterName::Default;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Encounter Defaults")
	int32 Level = 1;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> HealthBar;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> PassiveIndicatorComponent;

	/*
	 * AI 
	*/
	UPROPERTY() TObjectPtr<UBehaviorTree> BehaviorTree;
	UPROPERTY() TObjectPtr<ABaseAIController> BaseAIController;
	
	FVector StartLocation = FVector::ZeroVector;
	EBehaviourState BehaviourState = EBehaviourState::Patrol;
	EChasingState ChasingState = EChasingState::Chasing;
	bool bShouldAttack = true;
	
	/*
	 * AI Data
	*/
	EBehaviourType BehaviourType = EBehaviourType::Patrol;
	float SineMoveHeight = 0.f;
	float PatrolRadius = 750.f;
	float PatrolTickRadius = 450.f;
	float AttackRange = 300.f;
	float CloseRange = 275.f;
	float ChasingHeightOffset = 75.f;
	bool bCollisionCauseDamage = false;
	bool bShouldApplyInvincibility = false;	
	FValueRange AbilityPower;
	TSubclassOf<UGameplayEffect> DamageEffectClass;
	FGameplayTag DamageType;

public:
	UFUNCTION(BlueprintCallable)
	virtual EBehaviourState GetBehaviourState() override { return BehaviourState; }

	UFUNCTION(BlueprintCallable)
	virtual void SetBehaviourState(EBehaviourState NewState) override { BehaviourState = NewState; }
	
	virtual EMovementType GetMovementType_Implementation() override { return MovementType; }
	
};
