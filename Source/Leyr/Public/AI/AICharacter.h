// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "AIData.h"
#include "AI/BaseCharacter.h"
#include "Interaction/AIInterface.h"
#include "Interaction/EnemyInterface.h"
#include "UI/Controller/OverlayWidgetController.h"
#include "AICharacter.generated.h"

enum class EBehaviourType : uint8;
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
	/** end AI Interface */
	
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

protected:
	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo() override;
	virtual void InitializeDefaultAttributes() const override;	

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character Class Defaults")
	int32 Level = 1;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> HealthBar;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> PassiveIndicatorComponent;

	/*
	 * AI
	*/
	UPROPERTY(EditAnywhere, Category = "Character|AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;

	UPROPERTY()
	TObjectPtr<ABaseAIController> BaseAIController;
	
	UPROPERTY(EditAnywhere, Category = "Character|AI")
	EBehaviourType BehaviourType = EBehaviourType::Patrol;
	
	UPROPERTY(EditAnywhere, Category = "Character|AI")
	EBehaviourState BehaviourState = EBehaviourState::Patrol;
	
	UPROPERTY(EditAnywhere, Category = "Character|AI")
	EChasingState ChasingState = EChasingState::Chasing;
	
	UPROPERTY(EditAnywhere, Category = "Character|AI")
	float PatrolRadius = 750.f;
	
	UPROPERTY(EditAnywhere, Category = "Character|AI")
	float PatrolTickRadius = 450.f;
	
	UPROPERTY(EditAnywhere, Category = "Character|AI")
	float AttackRange = 300.f;
	
	UPROPERTY(EditAnywhere, Category = "Character|AI")
	float CloseRange = 275.f;
	
	FVector StartLocation = FVector::ZeroVector;

public:
	UFUNCTION(BlueprintCallable)
	virtual EBehaviourState GetBehaviourState() override { return BehaviourState; }
	
	UFUNCTION(BlueprintCallable)
	virtual void SetBehaviourState(EBehaviourState NewState) override { BehaviourState = NewState; }
};
