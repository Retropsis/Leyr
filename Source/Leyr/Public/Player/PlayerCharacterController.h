// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "Interaction/ControllerInterface.h"
#include "UI/Data/UIData.h"
#include "PlayerCharacterController.generated.h"

class UEquipmentWidgetController;
struct FWidgetControllerParams;
class UInventoryWidgetController;
class UWidget;
class UDamageTextComponent;
class UBaseAbilitySystemComponent;
struct FGameplayTag;
class APlayerCharacter;
class UInputAction;
class UInputConfig;
class UInputMappingContext;

/**
 * 
 */
UCLASS()
class LEYR_API APlayerCharacterController : public APlayerController, public IControllerInterface
{
	GENERATED_BODY()

public:
	virtual void PlayerTick(float DeltaTime) override;
	
	UInventoryWidgetController* GetInventoryWidgetController(const FWidgetControllerParams& WCParams);
	UInventoryWidgetController* GetInventoryWidgetController() { return InventoryWidgetController; }
	APlayerCharacter* GetPlayerCharacter();

	UFUNCTION(Client, Reliable)
	void ClientShowDamageNumber(const FUIMessageData& MessageData);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="PlayerCharacter")
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="PlayerCharacter")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="PlayerCharacter")
	TObjectPtr<UInputAction> JumpAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="PlayerCharacter")
	TObjectPtr<UInputAction> InventoryAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="PlayerCharacter")
	TObjectPtr<UInputAction> HotbarAction_0;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="PlayerCharacter")
	TObjectPtr<UInputAction> HotbarAction_1;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="PlayerCharacter")
	TObjectPtr<UInputAction> HotbarAction_2;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="PlayerCharacter")
	TObjectPtr<UInputAction> HotbarAction_3;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="PlayerCharacter")
	TObjectPtr<UInputAction> HotbarAction_4;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="PlayerCharacter")
	TObjectPtr<UInputAction> HotbarAction_5;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="PlayerCharacter")
	TObjectPtr<UInputAction> HotbarAction_6;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="PlayerCharacter")
	TObjectPtr<UInputAction> HotbarAction_7;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="PlayerCharacter")
	TObjectPtr<UInputAction> HotbarAction_8;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="PlayerCharacter")
	TObjectPtr<UInputAction> HotbarAction_9;
	
protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	
	void Move(const FInputActionValue& Value);
	void InteractButtonPressed(const FInputActionValue& Value);
	void Jump();
	void StopJumping();

	/*
	 * Inventory
	 */
	virtual void UpdateInventorySlot_Implementation(EContainerType ContainerType, int32 SlotIndex, FInventoryItemData ItemData) override {}
	virtual void UpdateContainerSlots_Implementation(int32 TotalSlots) override {}
	UFUNCTION(BlueprintCallable) virtual void ToggleContainer_Implementation(int32 SlotCount) override {}
	
	UFUNCTION(Client, Reliable) void InventoryButtonPressed();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable) void ToggleInventory();
	UFUNCTION(BlueprintCallable) void ToggleInputMode(UWidget* InWidgetToFocus);
	UPROPERTY(BlueprintReadWrite) bool bIsInventoryOpen = false;
	UPROPERTY(BlueprintReadWrite) bool bIsContainerOpen = false;
	
	UPROPERTY()
	TObjectPtr<UInventoryWidgetController> InventoryWidgetController;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UInventoryWidgetController> InventoryWidgetControllerClass;
	
	/*
	 * Hotbar
	 */
	void HotbarButtonPressed(int32 Index);

	/*
	 * Damage Numbers
	 */
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UDamageTextComponent> DamageTextComponentClass;

private:
	UPROPERTY()
	TObjectPtr<APlayerCharacter> PlayerCharacter;
	
	UPROPERTY()
	TObjectPtr<UBaseAbilitySystemComponent> BaseAbilitySystemComponent;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputConfig> InputConfig;

	FHitResult CursorHit;
	
	void CursorTrace();
	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);
	UBaseAbilitySystemComponent* GetASC();
};
