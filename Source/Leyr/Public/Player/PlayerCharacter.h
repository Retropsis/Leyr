// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "AI/BaseCharacter.h"
#include "Interaction/InventoryInterface.h"
#include "Interaction/PlayerInterface.h"
#include "PlayerCharacter.generated.h"

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
	
	void Move(const FVector2D MovementVector);
	void RotateController() const;
	void HandleCrouching(bool bShouldCrouch);
	void JumpButtonPressed();
	void TryVaultingDown();

	void TraceForLedge();
	void TraceForSlope();
	
	/** Combat Interface */
	virtual int32 GetCharacterLevel_Implementation() override;
	/** end Combat Interface */
	
	/** Inventory Interface */
	virtual UInventoryComponent* GetInventoryComponent_Implementation() override { return PlayerInventory; }
	/** end Inventory Interface */

	UFUNCTION(Server, Reliable)
	void ServerOnSlotDrop(EContainerType TargetContainer, EContainerType SourceContainer, int32 SourceSlotIndex, int32 TargetSlotIndex, EArmorType ArmorType);
	
	virtual void OnSlotDrop_Implementation(EContainerType TargetContainer, EContainerType SourceContainer, int32 SourceSlotIndex, int32 TargetSlotIndex, EArmorType ArmorType) override;

	/** Player Interface */
	virtual void ResetInventorySlot_Implementation(EContainerType ContainerType, int32 SlotIndex) override;
	virtual void UpdateInventorySlot_Implementation(EContainerType ContainerType, int32 SlotIndex, FInventoryItemData ItemData) override;	
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
	virtual ECombatState GetCombatState_Implementation() const override { return CombatState; }
	virtual void SetCombatState_Implementation(ECombatState NewState) override;
	virtual void SetMovementEnabled_Implementation(bool Enabled) override;
	virtual void SetComboWindow_Implementation(bool bOpen) override { bIsComboWindowOpen = bOpen; }
	virtual bool IsComboWindowOpen_Implementation() override { return bIsComboWindowOpen; }
	virtual void ResetAttacking_Implementation() override {}
	virtual void ResetCombatVariables_Implementation() override {}
	virtual int32 GetAttackComboIndex_Implementation() override { return AttackCount; }
	virtual void SetAttackComboIndex_Implementation(int32 Index) override { AttackCount = Index; }
	virtual FTaggedMontage GetTaggedMontageByIndex_Implementation(int32 Index) override;
	virtual ECombatState GetPlayerCombatState_Implementation() override { return CombatState; }
	virtual void SetPlayerCombatState_Implementation(const ECombatState NewState) override { CombatState = NewState; }
	virtual void HandleHangingOnLadder_Implementation(FVector HangingTarget, bool bEndOverlap) override;
	virtual void HandleHangingOnRope_Implementation(FVector HangingTarget, bool bEndOverlap) override;
	virtual void SetMovementTarget_Implementation(const FVector Target) override { MovementTarget = Target; } 
	/** end Player Interface */

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UNiagaraComponent> LevelUpNiagaraComponent;

protected:
	virtual void InitAbilityActorInfo() override;
	void HandleCombatState(ECombatState NewState);
	void HandleHangingOnLedge(const FVector& HangingTarget);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Player")
	float LadderWalkSpeed = 120.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Player")
	float RopeWalkSpeed = 120.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Player")
	float GroundSlopeWalkSpeed = 20.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Player")
	float ClimbingSpeed = 3.f;

	FTimerHandle OffLedgeTimer;
	float OffLedgeTime = .5f;
	bool bCanGrabLedge = true;
	UFUNCTION() void OffLedgeEnd();
	
	bool bAirborne = false;
	bool bIsAccelerating = false;
	bool bIsMoving = false;
	bool bIsComboWindowOpen = false;

	UPROPERTY(BlueprintReadWrite)
	int32 AttackCount = 0;
	
	ECombatState CombatState = ECombatState::Unoccupied;
	ECombatState PreviousCombatState = ECombatState::Unoccupied;

private:	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Player", meta=(AllowPrivateAccess="true"))
	TObjectPtr<USpringArmComponent> SpringArm;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Player", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UCameraComponent> FollowCamera;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Inventory", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UInventoryComponent> PlayerInventory;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Inventory", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UHotbarComponent> HotbarComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Player|Plaforming", meta=(AllowPrivateAccess="true"))
	TObjectPtr<USceneComponent> GroundPoint;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Player|Plaforming", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UBoxComponent> RopeHangingCollision;

	UPROPERTY(EditDefaultsOnly, Category="Player|Plaforming")
	float PlatformTraceDistance = 10.f;

	UFUNCTION(NetMulticast, Reliable)
	void MulticastLevelUpParticles() const;

	void TraceForPlatforms() const;
	void OverlapPlatformEnd();

	FTimerHandle OverlapPlatformTimer;
	bool bOverlapPlatformTimerEnded = true;

	UPROPERTY(EditAnywhere, Category="Player|Plaforming", meta=(AllowPrivateAccess="true"))
	float OverlapPlatformTime = .25f;

	FVector MovementTarget = FVector::ZeroVector;

public:
	FORCEINLINE bool IsAirborne() const { return bAirborne; }
	FORCEINLINE bool IsAccelerating() const { return bIsAccelerating; }
	FORCEINLINE bool IsMoving() const { return bIsMoving; }
};
