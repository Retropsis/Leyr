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

class UEncounterData;
class AArena;
class ANavMeshBoundsVolume;
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
	void InitializeNavigationBounds();
	virtual void PossessedBy(AController* NewController) override;
	virtual void HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount) override;
	void HandleBehaviourState(EBehaviourState NewState);
	void HandlePlayerOverlappingArena(AActor* Player, bool bIsEntering);

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
	
	/** Combat Interface */
	virtual int32 GetCharacterLevel_Implementation() override { return Level; }
	virtual void SetCombatTarget_Implementation(AActor* InCombatTarget) override;
	virtual AActor* GetCombatTarget_Implementation() const override { return CombatTarget; }
	virtual void Die(const FVector& DeathImpulse, bool bExecute) override;
	/** end Combat Interface */

	UFUNCTION()
	void HandleCombatTargetDefeated(AActor* Actor);
	
	/** AI Interface */
	virtual FVector FindRandomLocation_Implementation() override;
	virtual bool MoveToLocation_Implementation(FVector TargetLocation, float Threshold) override;
	virtual bool ChaseTarget_Implementation(AActor* TargetToChase) override;
	virtual void SineMove_Implementation() override;
	virtual void FaceTarget_Implementation() override;
	virtual bool FollowSpline_Implementation(int32 SplineIndex) override;
	virtual FVector GetNextLocation_Implementation(int32 SplineIndex) override;
	virtual void StartSplineMovement_Implementation() override;
	virtual bool FollowSplinePoints_Implementation(int32 SplineIndex) override;
	virtual void SetNewMovementSpeed_Implementation(EMovementMode InMovementMode, float NewSpeed) override;
	virtual EEncounterSize GetEncounterSize_Implementation() override { return EncounterSize; }
	virtual FName GetNextBehaviourPattern_Implementation(FName PatternName) override;
	FBoundLocations CalculateBoundsAtActorZ() const;
	virtual FBoundLocations GetArenaBounds_Implementation() override;
	virtual bool IsWithinBounds_Implementation(const FVector& Location) override;
	virtual FBoxSphereBounds GetEnteringBounds_Implementation() override;
	virtual bool IsTargetWithinEnteringBounds_Implementation(const FVector& Location) override;
	/** end AI Interface */
	
	bool IsTargetWithinEnteringBounds(const FVector& TargetLocation) const;

	/** Enemy Interface */
	virtual void SetShouldAttack_Implementation(bool InShouldAttack) override;
	virtual bool ShouldAttack_Implementation() const override { return bShouldAttack; }
	/** Enemy Interface */

	void SetEncounterClass(const ECharacterClass NewClass) { CharacterClass = NewClass; }
	void SetEncounterLevel(const int32 InLevel) { Level = InLevel; }
	void SetEncounterData(UEncounterData* Data) { EncounterData = Data; }
	
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
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character|AI")
	TObjectPtr<ASplineComponentActor> SplineComponentActor;
	
	UPROPERTY(EditAnywhere, Category = "Character|AI")
	TObjectPtr<ANavMeshBoundsVolume> NavMeshBoundsVolume;
	
	UPROPERTY(EditAnywhere, Category = "Character|AI")
	bool bShouldBuildNavMesh = false;
	
	TObjectPtr<USplineComponent> SplineComponent;
	TArray<FVector> SplinePoints;

	UPROPERTY(EditAnywhere, Category = "Character|AI")
	EMovementType MovementType = EMovementType::Destination;

	UPROPERTY(EditInstanceOnly, Category = "Character|AI")
	TObjectPtr<AArena> Arena = nullptr;

	// TODO: Add this to Data
	UPROPERTY(EditDefaultsOnly, Category = "Character|AI")
	TArray<FGameplayTag> BehaviourPatterns;

protected:
	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo() override;
	virtual void InitializeDefaultAttributes() const override;
	virtual void InitializeCharacterInfo() override;
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
	UPROPERTY(EditDefaultsOnly, Category="AI")
	TObjectPtr<UEncounterData> EncounterData;
	
	EBehaviourType BehaviourType = EBehaviourType::Patrol;
	EEncounterSize EncounterSize = EEncounterSize::Default;
	float SineMoveHeight = 0.f;
	float PatrolRadius = 750.f;
	float PatrolTickRadius = 450.f;
	float AttackRange = 300.f;
	float AttackCooldown = 5.f;
	float CloseRange = 275.f;
	float ChasingSpeed = 250.f;
	float DivingSpeed = 750.f;
	float ChasingHeightOffset = 75.f;
	bool bCollisionCauseDamage = false;
	bool bShouldApplyInvincibility = false;
	//~ Collision Damage
	float AbilityPower;
	FGameplayTag DamageType;
	TSubclassOf<UGameplayEffect> DamageEffectClass;
	//~ Bounds
	FBoxSphereBounds EnteringBounds{};
	FBoxSphereBounds NavigationBounds{};

private:
	void ShouldAttack(bool InShouldAttack);
	FTimerHandle ResetAttackTimer;
	
public:
	UFUNCTION(BlueprintCallable)
	virtual EBehaviourState GetBehaviourState() override { return BehaviourState; }

	UFUNCTION(BlueprintCallable)
	virtual void SetBehaviourState(EBehaviourState NewState) override { BehaviourState = NewState; }
	
	virtual EMovementType GetMovementType_Implementation() override { return MovementType; }
	
};
