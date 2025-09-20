// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Data/CharacterInfo.h"
#include "AI/BaseCharacter.h"
#include "World/Data/CameraData.h"
#include "Interaction/InventoryInterface.h"
#include "Interaction/PlayerInterface.h"
#include "Inventory/Container/Container.h"
#include "PlayerCharacter.generated.h"

class AParallaxController;
enum class ECharacterName : uint8;
class UCharacterInfo;
class UBoxComponent;
class UHotbarComponent;
class UNiagaraComponent;
struct FInventoryItemData;
class UPlayerInventoryComponent;
class UCameraComponent;
class USpringArmComponent;

/**
 * 
 */
UCLASS()
class LEYR_API APlayerCharacter : public ABaseCharacter, public IPlayerInterface, public IInventoryInterface
{
	GENERATED_BODY()

public:
	APlayerCharacter();
	virtual void Tick(float DeltaSeconds) override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;
	virtual void HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount) override;
	virtual void PeacefulTagChanged(const FGameplayTag CallbackTag, int32 NewCount);
	virtual void SwimmingTagChanged(const FGameplayTag CallbackTag, int32 NewCount);
	virtual void RuleTagChanged(const FGameplayTag CallbackTag, int32 NewCount);
	virtual void OnStartCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust) override;
	virtual void OnEndCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust) override;
	virtual void Die(const FVector& DeathImpulse, bool bExecute) override;
	
	void Move(const FVector2D MovementVector);
	void Pitch(float InPitch);
	void RotateController() const;
	bool TryDescendLadder();
	void TryClimbingRope(FVector2D MovementVector);
	bool TryVaultingDown();
	void HandleCrouching(bool bShouldCrouch);
	void JumpButtonPressed();

	UFUNCTION()
	void HandleCharacterMovementUpdated(float DeltaSeconds, FVector OldLocation, FVector OldVelocity);
	void InitializeParallaxController();
	EMovementMode PreviousMovementMode = MOVE_None;

	void TraceForLedge();
	void TraceForSlope();
	void TraceForPlatforms() const;
	void TraceForHoppingLedge(float MovementVectorX);

	/*
	 * Camera
	 */
	void InterpCameraAdditiveOffset(float DeltaTime);
	
	UFUNCTION(BlueprintPure)
	FVector GetCameraOffsetLocation() const;
	
	UPROPERTY() TObjectPtr<AActor> ActorToInterp;
	FVector TargetToInterp;
	FVector LastActorToFollowInterpLocation;
	float InterpCameraAlpha = 0.f;
	float CurrentTargetDistance = 0.f;
	bool bShouldUseCameraExtents = false;
	FCameraBounds CameraBounds;
	FBoxSphereBounds CameraEnteringBounds;
	ECameraInterpState CameraInterpState = ECameraInterpState::None;

	UPROPERTY(EditDefaultsOnly)
	float FollowingInterpSpeed = 1.f;
	
	UPROPERTY(EditDefaultsOnly)
	float EnteringInterpSpeed = 8.f;
	
	UPROPERTY(EditDefaultsOnly)
	float SnapDistance = 35.f;
	
	UPROPERTY(EditDefaultsOnly)
	float ExitingInterpSpeed = 8.f;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UCurveTable> ScalableInterpSpeedCurve;
	
	UPROPERTY(EditDefaultsOnly)
	float ActorToFollowMaxDistance = 1000.f;
	
	/** Combat Interface */
	virtual int32 GetCharacterLevel_Implementation() override;
	virtual ECombatState GetCombatState_Implementation() const override { return CombatState; }
	virtual void SetCombatState_Implementation(ECombatState NewState) override;
	virtual void SetCombatStateToHandle_Implementation(ECombatState NewState, const FCombatStateParams& Params) override;
	virtual void SetComboWindow_Implementation(bool bOpen) override { bIsComboWindowOpen = bOpen; }
	virtual bool IsComboWindowOpen_Implementation() override { return bIsComboWindowOpen; }
	virtual void ResetAttacking_Implementation() override {}
	virtual void ResetCombatVariables_Implementation() override {}
	virtual int32 GetAttackComboIndex_Implementation() override { return AttackCount; }
	virtual void SetAttackComboIndex_Implementation(int32 Index) override { AttackCount = Index; }
	virtual ECombatState GetPlayerCombatState_Implementation() override { return CombatState; }
	virtual void SetPlayerCombatState_Implementation(const ECombatState NewState) override { CombatState = NewState; }
	virtual void SetMovementTarget_Implementation(const FVector Target) override { MovementTarget = Target; } 
	virtual FTaggedMontage GetTaggedMontageByIndex_Implementation(int32 Index) override;
	virtual float GetOverridePitch_Implementation() override { return OverridePitch; }
	virtual void ResetAimingPitch_Implementation() override;
	/** end Combat Interface */
	
	int32 GetPlayerLevel() const;
	
	/** Inventory Interface */
	virtual UInventoryComponent* GetInventoryComponent_Implementation() override { return PlayerInventory; }
	/** end Inventory Interface */
	virtual UInventoryComponent* GetPlayerInventory() { return PlayerInventory; }
	virtual UInventoryComponent* GetInteractingContainer() { return InteractingContainer ? InteractingContainer->Container : nullptr; }
	
	UFUNCTION(Server, Reliable)
	void ServerInteract();
	
	UFUNCTION(Server, Reliable)
	void ServerCloseContainer(); 

	UFUNCTION(Server, Reliable)
	void ServerOnSlotDrop(EContainerType TargetContainer, EContainerType SourceContainer, int32 SourceSlotIndex, int32 TargetSlotIndex, EArmorType ArmorType);
	virtual void OnSlotDrop_Implementation(EContainerType TargetContainer, EContainerType SourceContainer, int32 SourceSlotIndex, int32 TargetSlotIndex, EArmorType ArmorType) override;
	virtual AContainer* GetContainer_Implementation() override { return InteractingContainer; }

	/** Player Interface */
	virtual USpringArmComponent* GetSpringArmComponent_Implementation() override { return SpringArm; }
	virtual UCameraComponent* GetCameraComponent_Implementation() override { return FollowCamera; }
	virtual void SetCameraInterpolation_Implementation(ACameraBoundary* CameraBoundary, ECameraInterpState NewState) override;
	virtual void DisableCameraLagForDuration_Implementation(float Duration) override;
	
	virtual void ResetInventorySlot_Implementation(EContainerType ContainerType, int32 SlotIndex) override;
	virtual void UpdateInventorySlot_Implementation(EContainerType ContainerType, int32 SlotIndex, FInventoryItemData ItemData) override;
	virtual void UpdateContainerSlots_Implementation(int32 TotalSlots) override;
	virtual void SetContainer_Implementation(AContainer* Container) override;
	virtual void CloseContainer_Implementation() override;
	virtual UInventoryComponent* GetInventoryComponentByType_Implementation(EContainerType Type) override;
	virtual bool UseItem_Implementation(UItemData* Asset, int32 Amount, bool bIsSelfCost, int32& OutQuantity) override;
	virtual void TryOpenKeylock_Implementation(const TSoftObjectPtr<UItemData>& Asset) override;
	virtual FOnKeyItemUsed& GetOnKeyItemUsed() override { return OnKeyItemUsed; }
	
	virtual void AddToXP_Implementation(int32 InXP) override;
	virtual void LevelUp_Implementation() override;
	virtual int32 GetXP_Implementation() const override;
	virtual int32 FindLevelForXP_Implementation(int32 InXP) const override;
	virtual int32 GetAttributePointsReward_Implementation(int32 Level) const override;
	virtual int32 GetSkillPointsReward_Implementation(int32 Level) const override;
	virtual void AddToPlayerLevel_Implementation(int32 InPlayerLevel) override;
	virtual void AddToAttributePoints_Implementation(int32 InAttributePoints) override;
	virtual void AddToSkillPoints_Implementation(int32 InSpellPoints) override;
	virtual int32 GetAttributePoints_Implementation() const override;
	virtual int32 GetSkillPoints_Implementation() const override;
	
	virtual void HandleHangingOnLadder_Implementation(FVector HangingTarget, bool bEndOverlap) override;
	virtual void HandleHangingOnRope_Implementation(FVector HangingTarget, bool bEndOverlap) override;
	virtual void HandleHangingOnHook_Implementation(FVector HangingTarget, bool bEndOverlap) override;
	virtual void HandleClimbing_Implementation(FVector HangingTarget, bool bEndOverlap) override;
	virtual void HandleEntangled_Implementation(float MinZ, float EntangledWalkSpeed, float EntangledGravityScale, bool bEndOverlap) override;
	virtual void HandleSwimming_Implementation(float MinZ, float EnvironmentSwimmingSpeed, float SwimmingGravityScale, bool bEndOverlap) override;
	virtual void HandleElevator_Implementation(APawn* InElevator, bool bEndOverlap) override;
	virtual void HandleOverlapZone_Implementation(ECombatState NewState, bool bEndOverlap) override;
	virtual void ReduceWalkSpeed_Implementation(float AmountToReduce) override;
	virtual void SetWalkSpeed_Implementation(float NewSpeed) override;
	virtual void ToggleAiming_Implementation(bool bAiming) override;

	virtual void SaveProgress_Implementation(const FName& SavePointTag) override;
	/** end Player Interface */
	void LoadProgress();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UNiagaraComponent> LevelUpNiagaraComponent;

protected:
	virtual void InitAbilityActorInfo() override;
	virtual void InitializeCharacterInfo() override;
	virtual void AddCharacterAbilities() override;
	void HandleCombatState(ECombatState NewState, const FCombatStateParams& Params = FCombatStateParams());
	void HandleHangingOnLedge(const FVector& HangingTarget);
	void InitializeCameraBoundary();
	void ClampToCameraBounds(FVector& PreferredCameraLocation) const;
	bool IsWithinCameraBounds(const FVector& InLocation) const;
	virtual void BeginPlay() override;
	
	UPROPERTY(EditDefaultsOnly, Category="Player", meta=(Categories=Player))
	FGameplayTag CharacterTag = FGameplayTag();
	
	UPROPERTY(EditDefaultsOnly, Category="Player")
	TObjectPtr<UCharacterInfo> CharacterInfo = nullptr;

	/*
	 * Movement
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Player|Movement")
	float BaseRunSpeed = 450.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Player|Movement")
	float LadderWalkSpeed = 120.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Player|Movement")
	float RopeWalkSpeed = 120.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Player|Movement")
	float ClimbingWalkSpeed = 120.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Player|Movement")
	float AimingWalkSpeed = 80.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Player|Movement")
	float SwimmingSpeed = 225.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Player|Movement")
	float GroundSlopeGravityScale = 1.25f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Player|Movement")
	float ClimbingSpeed = 3.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Player|Movement")
	float DodgingSpeed = 2400.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Player|Movement")
	float DodgingMaxAcceleration = 5000.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Player|Movement")
	float DodgingBrakeFrictionFactor = .2f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Player|Movement")
	float RollingSpeed = 1000.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Player|Movement")
	float RollingMaxAcceleration = 5000.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Player|Movement")
	float RollingBrakeFrictionFactor = 500.f;

	FTimerHandle OffLedgeTimer;
	float OffLedgeTime = .2f;
	bool bCanGrabLedge = true;
	
	FTimerHandle UnCrouchingTimer;
	float UnCrouchingTime = .25f;
	
	bool bAirborne = false;
	bool bIsAccelerating = false;
	bool bIsMoving = false;
	bool bCrouchButtonHeld = false;

	UPROPERTY(BlueprintReadWrite)
	int32 AttackCount = 0;
	
	UPROPERTY(BlueprintReadWrite)
	bool bIsComboActivated = false;
	
	UPROPERTY(BlueprintReadWrite)
	bool bIsComboWindowOpen = false;
	
	ECombatState CombatState = ECombatState::Unoccupied;
	ECombatState PreviousCombatState = ECombatState::Unoccupied;
	ECombatDirection CombatDirection = ECombatDirection::None;
	ECombatDirection PreviousCombatDirection = ECombatDirection::None;

	bool bApplyTag = false;

	ECombatDirection GetCombatDirectionFromVector2D(FVector2D MovementVector);
	void HandleCombatDirectionTag() const;
	
	virtual void OnRep_Stunned() override;
	virtual void OnRep_Burned() override;

private:			
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Player", meta=(AllowPrivateAccess="true"))
	TObjectPtr<USpringArmComponent> SpringArm;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Player", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UCameraComponent> FollowCamera;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Player", meta=(AllowPrivateAccess="true"))
	TObjectPtr<USpringArmComponent> ParallaxSpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Player", meta=(AllowPrivateAccess="true"))
	TObjectPtr<AParallaxController> ParallaxController;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Inventory", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UInventoryComponent> PlayerInventory;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Inventory", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UHotbarComponent> HotbarComponent;

	UPROPERTY()
	TObjectPtr<ACameraBoundary> CurrentCameraBoundary;

	UPROPERTY()
	TObjectPtr<AContainer> InteractingContainer;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Player|Plaforming", meta=(AllowPrivateAccess="true"))
	TObjectPtr<USceneComponent> GroundPoint;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Player|Plaforming", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UBoxComponent> RopeHangingCollision;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Player|Plaforming", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UBoxComponent> SwimmingCollision;

	UPROPERTY(EditDefaultsOnly, Category="Player|Plaforming")
	float PlatformTraceDistance = 15.f;

	UPROPERTY(EditAnywhere, Category="Player|Plaforming", meta=(AllowPrivateAccess="true"))
	float OverlapPlatformTime = .25f;

	UPROPERTY(EditAnywhere, Category="Player|Plaforming", meta=(AllowPrivateAccess="true"))
	float EntangledExitTime = .1f;
	
	UPROPERTY(EditAnywhere, Category="Player|Plaforming", meta=(AllowPrivateAccess="true"))
	float SwimmingExitTime = .09f;
	
	UPROPERTY(EditAnywhere, Category="Player|Combat", meta=(AllowPrivateAccess="true"))
	float ForgetOverridePitchTime = 3.f;
	
	UFUNCTION(NetMulticast, Reliable)
	void MulticastLevelUpParticles() const;
	
	FTimerHandle EntangledExitTimer;
	FTimerHandle OverlapPlatformTimer;
	FTimerHandle ForgetOverridePitchTimer;
	
	bool bOverlapPlatformTimerEnded = true;
	FVector MovementTarget = FVector::ZeroVector;
	float MovementSpeed = 0.f;
	float CurrentMinZ = 0.f;
	float OverridePitch = 0.f;

	UPROPERTY()
	APawn* Elevator = nullptr;

	UPROPERTY(EditDefaultsOnly)
	float DefeatTime = 2.f;

	FTimerHandle DefeatTimer;

	FVector CurrentAdditiveOffset;
	FVector PreviousAdditiveOffset;
	float LastActorToInterpDistance;
	
	FOnKeyItemUsed OnKeyItemUsed;

	bool bWantsToUncrouch = false;

public:
	FORCEINLINE bool IsAirborne() const { return bAirborne; }
	FORCEINLINE bool IsAccelerating() const { return bIsAccelerating; }
	FORCEINLINE bool IsMoving() const { return bIsMoving; }
};
