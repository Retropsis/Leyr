// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "AI/BaseCharacter.h"
#include "AIData.h"
#include "Data/EncounterData.h"
#include "Interaction/AIInterface.h"
#include "Interaction/EnemyInterface.h"
#include "UI/Controller/OverlayWidgetController.h"
#include "AICharacter.generated.h"

class UBehaviourData;
class UTimelineComponent;
enum class EBehaviourType : uint8;
class ULootDataSet;
class UEncounterData;
class AArena;
class ANavMeshBoundsVolume;
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
	void InitializeNavigationBounds();
	void HandleBehaviourState(EBehaviourState NewState);
	void HandlePlayerOverlappingArena(AActor* Player, bool bIsEntering);

	UFUNCTION(BlueprintCallable)
	void HandleBeginOverlap(AActor* OtherActor);

	UFUNCTION(BlueprintImplementableEvent)
	void SetupCollisionDamage();
	
	/** Combat Interface */
	virtual int32 GetCharacterLevel_Implementation() override { return Level; }
	virtual void SetCombatTarget_Implementation(AActor* InCombatTarget) override;
	virtual AActor* GetCombatTarget_Implementation() const override { return CombatTarget; }
	virtual void Die(const FVector& DeathImpulse, bool bExecute) override;
	/** end Combat Interface */

	UFUNCTION()
	void HandleCombatTargetDefeated(AActor* Actor);

	UFUNCTION(BlueprintPure)
	UAnimInstance* FindMultiPartAnimInstanceForTag(const FGameplayTag& Tag);
	
	UFUNCTION(BlueprintPure)
	UPaperZDAnimInstance* FindPaperAnimInstanceForTag(const FGameplayTag& Tag);
	
	UFUNCTION(BlueprintPure)
	ASplineComponentActor* FindSplineForTag(const FGameplayTag& Tag);

	virtual void SetMultiPartAnimInstance(TSubclassOf<UAnimInstance> NewInstance) {}
	
	/** AI Interface */
	virtual void ChangePaperAnimInstance_Implementation(TSubclassOf<UPaperZDAnimInstance> NewInstance) override;
	virtual UAnimInstance* FindMultiPartAnimInstanceForTagName_Implementation(const FName& TagName) override;
	virtual UPaperZDAnimInstance* FindPaperAnimInstanceForTagName_Implementation(const FName& TagName) override;
	virtual ASplineComponentActor* FindSplineForTagName_Implementation(const FName& TagName) override;
	virtual void FindAndApplyPatternParamsForPattern_Implementation(const FName& PatternName) override;
	
	virtual FVector FindRandomLocation_Implementation() override;
	virtual bool MoveToLocation_Implementation(FVector TargetLocation, float Threshold, bool bBackward = false) override;
	virtual FVector FindTargetLocation_Implementation(AActor* TargetActor, float DistanceToKeep) override;
	virtual bool CheckForObstacle_Implementation(float TraceDistance = 50.f, bool bBackward = false) override;
	virtual bool CheckForGround_Implementation(float TraceDistance = 175.f, bool bBackward = false) override;
	virtual bool RequestJump_Implementation() override;
	virtual bool ChaseTarget_Implementation(AActor* TargetToChase) override;
	virtual bool ChaseTargetWithinWater_Implementation(AActor* TargetToChase) override;
	virtual void SetupFlyAroundTarget_Implementation(FVector TargetLocation, float Radius = 225.f) override;
	virtual void FlyAroundTarget_Implementation() override;
	virtual void StartTimelineMovement_Implementation() override;
	virtual void SetJumpApex_Implementation(float NewApex) override { JumpApex = NewApex; }
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
	virtual FBoxSphereBounds GetEnteringBounds_Implementation() override { return EnteringBounds; }
	virtual FBoxSphereBounds GetNavigationBounds_Implementation() override { return NavigationBounds; }
	virtual FBoundLocations GetSpawningBounds_Implementation() override { return SpawningBounds; }
	virtual void SetSpawningBounds_Implementation(const FBoundLocations NewBounds) override { SpawningBounds = NewBounds; }
	virtual bool IsTargetWithinEnteringBounds_Implementation(const FVector& Location) override;
	virtual bool ShouldDespawn_Implementation() override { return bShouldDespawn; }
	/** end AI Interface */
	
	bool IsTargetWithinEnteringBounds(const FVector& TargetLocation) const;
	bool IsTargetWithinNavigationBounds(const FVector& TargetLocation) const;

	/** Enemy Interface */
	virtual void SetShouldAttack_Implementation(bool InShouldAttack) override;
	virtual bool ShouldAttack_Implementation() const override { return bShouldAttack; }
	/** Enemy Interface */

	void SetEncounterClass(const ECharacterClass NewClass) { CharacterClass = NewClass; }
	void SetEncounterLevel(const int32 InLevel) { Level = InLevel; }
	void SetEncounterData(UEncounterData* Data) { EncounterData = Data; }
	void SetBehaviourData(UBehaviourData* Data) const { if (EncounterData) EncounterData->BehaviourData = Data; }
	
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
	ASplineComponentActor* SplineComponentActor;
	
	UPROPERTY(EditDefaultsOnly, Category = "Character|AI")
	TArray<FGameplayTag> BehaviourPatterns;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character|AI")
	TMap<FGameplayTag, FPatternParams> PatternParams;
	
	UPROPERTY(EditAnywhere, Category = "Character|AI")
	TObjectPtr<ANavMeshBoundsVolume> NavMeshBoundsVolume;
	
	UPROPERTY(EditAnywhere, Category = "Character|AI")
	bool bShouldBuildNavMesh = false;

	UPROPERTY()
	TObjectPtr<USplineComponent> SplineComponent;
	
	TArray<FVector> SplinePoints;

	UPROPERTY(EditAnywhere, Category = "Character|AI")
	EMovementType MovementType = EMovementType::Destination;

	UPROPERTY(EditInstanceOnly, Category = "Character|AI")
	TObjectPtr<AArena> Arena = nullptr;

protected:
	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo() override;
	virtual void InitializeDefaultAttributes() const override;
	virtual void InitializeCharacterInfo() override;
	virtual void AddCharacterAbilities() override;
	virtual void StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount) override;
	
	virtual void OnRep_Stunned() override;
	virtual void OnRep_Burned() override;
	
	UFUNCTION(BlueprintCallable)
	void CauseDamage(AActor* TargetActor);

	UFUNCTION(BlueprintCallable)
	void ResetShouldAttack();
	
	UFUNCTION(BlueprintImplementableEvent)
	void SpawnLoot();
	
	UFUNCTION()
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
		
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

	// Timeline Movement
	UPROPERTY(BlueprintReadOnly)
	FVector InitialTimelineLocation = FVector::ZeroVector;
	
	UPROPERTY(BlueprintReadWrite)
	FVector EndTimelineLocation = FVector::ZeroVector;
	
	UPROPERTY(BlueprintReadOnly)
	float JumpApex = 100.f;

	UFUNCTION(BlueprintCallable)
	void InitializeSineMove();
	
	UFUNCTION(BlueprintCallable)
	void ShouldEndSineMove(UTimelineComponent* Timeline);
	
	void RandomAmplitudeFromRange(float Min = 25.f, float Max = 100.f);
	
	UPROPERTY(BlueprintReadOnly)
	float SineAmplitude = 0.f;

	bool bExitSineMove = false;
	
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FEndTimelineMovement EndTimelineMovement;

	// Fly Around Target
	FVector FlyAroundTargetLocation = FVector::ZeroVector;
	float FlyAroundRadius = 0.f;
	float FlyAroundDeltaTimeStart = 0.f;
	
	/*
	 * AI Data
	*/
	UPROPERTY(EditDefaultsOnly, Category="AI")
	TObjectPtr<UEncounterData> EncounterData;
	
	EBehaviourType BehaviourType = EBehaviourType::Patrol;
	EEncounterSize EncounterSize = EEncounterSize::Default;
	float SineMoveHeight = 0.f;
	float SineMoveSpeed = 0.f;
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
	FBoundLocations SpawningBounds{};
	bool bShouldDespawn = false;
	//~ LootData
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<ULootDataSet> LootData = nullptr;

private:
	void ShouldAttack(bool InShouldAttack);
	FTimerHandle ResetAttackTimer;
	
public:
	UFUNCTION(BlueprintCallable)
	virtual EBehaviourState GetBehaviourState() override { return BehaviourState; }

	UFUNCTION(BlueprintCallable)
	virtual void SetBehaviourState(EBehaviourState NewState) override { BehaviourState = NewState; }
	
	virtual EMovementType GetMovementType_Implementation() override { return MovementType; }

	UFUNCTION(BlueprintPure) bool ShouldCollisionCauseDamage() const { return bCollisionCauseDamage; }
	
	virtual ECharacterClass GetEncounterClass() override { return CharacterClass; }
	virtual int32 GetEncounterLevel() override { return Level; }
};
