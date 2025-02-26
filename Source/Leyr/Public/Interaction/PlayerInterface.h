// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "Inventory/InventoryComponent.h"
#include "UObject/Interface.h"
#include "PlayerInterface.generated.h"

class UCameraComponent;
class USpringArmComponent;
class AContainer;
enum class ECombatState : uint8;
enum class EArmorType : uint8;
enum class EContainerType : uint8;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPlayerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class LEYR_API IPlayerInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	/*
	 * Camera
	 */
	UFUNCTION(BlueprintNativeEvent)
	USpringArmComponent* GetSpringArmComponent();
	
	UFUNCTION(BlueprintNativeEvent)
	UCameraComponent* GetCameraComponent();

	UFUNCTION(BlueprintNativeEvent)
	void ToggleCameraInterpToActor(AActor* InActorToFollow, bool bToggle);
	
	/*
	 * Ability System
	 */
	UFUNCTION(BlueprintNativeEvent)
	int32 FindLevelForXP(int32 InXP) const;

	UFUNCTION(BlueprintNativeEvent)
	int32 GetXP() const;

	UFUNCTION(BlueprintNativeEvent)
	int32 GetAttributePointsReward(int32 Level) const;

	UFUNCTION(BlueprintNativeEvent)
	int32 GetAttributePoints() const;
	
	UFUNCTION(BlueprintNativeEvent)
	int32 GetSkillPointsReward(int32 Level) const;

	UFUNCTION(BlueprintNativeEvent)
	int32 GetSkillPoints() const;
	
	UFUNCTION(BlueprintNativeEvent)
	void AddToXP(int32 InXP);

	UFUNCTION(BlueprintNativeEvent)
	void AddToPlayerLevel(int32 InPlayerLevel);

	UFUNCTION(BlueprintNativeEvent)
	void AddToAttributePoints(int32 InAttributePoints);

	UFUNCTION(BlueprintNativeEvent)
	void AddToSkillPoints(int32 InSkillPoints);
	
	UFUNCTION(BlueprintNativeEvent)
	void LevelUp();

	/*
	 * Ability
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ToggleAiming(bool bAiming);

	/*
	 * Inventory
	 */
	UFUNCTION(BlueprintNativeEvent)
	void UpdateInventorySlot(EContainerType ContainerType, int32 SlotIndex, FInventoryItemData ItemData);

	UFUNCTION(BlueprintNativeEvent)
	void ResetInventorySlot(EContainerType ContainerType, int32 SlotIndex);

	UFUNCTION(BlueprintNativeEvent)
	void UpdateContainerSlots(int32 TotalSlots);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnSlotDrop(EContainerType TargetContainer, EContainerType SourceContainer, int32 SourceSlotIndex, int32 TargetSlotIndex, EArmorType ArmorType);
	
	UFUNCTION(BlueprintNativeEvent)
	void SetContainer(AContainer* Container);
	
	UFUNCTION(BlueprintNativeEvent)
	AContainer* GetContainer();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UInventoryComponent* GetInventoryComponentByType(EContainerType Type);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void CloseContainer();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool UseItem(UItemData* Asset, int32 Amount, bool bIsSelfCost, int32& OutQuantity);
	
	/*
	 * Combat
	 */
	UFUNCTION(BlueprintNativeEvent)
	ECombatState GetPlayerCombatState();
	
	UFUNCTION(BlueprintNativeEvent)
	void SetPlayerCombatState(const ECombatState NewState);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetIsAttack(bool bIsAttacking);
	
	UFUNCTION(BlueprintNativeEvent)
	void ReduceWalkSpeed(float AmountToReduce);
	
	UFUNCTION(BlueprintNativeEvent)
	void SetWalkSpeed(float NewSpeed);

	UFUNCTION(BlueprintNativeEvent)
	void HandleHangingOnLadder(FVector HangingTarget, bool bEndOverlap);

	UFUNCTION(BlueprintNativeEvent)
	void HandleHangingOnRope(FVector HangingTarget, bool bEndOverlap);

	UFUNCTION(BlueprintNativeEvent)
	void HandleHangingOnHook(FVector HangingTarget, bool bEndOverlap);
	
	UFUNCTION(BlueprintNativeEvent)
	void HandleClimbing(FVector HangingTarget, bool bEndOverlap);

	UFUNCTION(BlueprintNativeEvent)
	void HandleEntangled(float MinZ, float EntangledWalkSpeed, float EntangledGravityScale, bool bEndOverlap);

	UFUNCTION(BlueprintNativeEvent)
	void HandleSwimming(float MinZ, float SwimmingSpeed, float SwimmingGravityScale, bool bEndOverlap);
	
	UFUNCTION(BlueprintNativeEvent)
	void HandleElevator(APawn* InElevator, bool bEndOverlap);
	
	UFUNCTION(BlueprintNativeEvent)
	void HandleOverlapZone(ECombatState NewState, bool bEndOverlap);

	/*
	 * Saving
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SaveProgress(const FName& SavePointTag);
};
