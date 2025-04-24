// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Interaction/ControllerInterface.h"
#include "InputActionValue.h"
#include "UI/Data/UIData.h"
#include "CommonUserWidget.h"
#include "PlayerCharacterController.generated.h"

struct FWidgetControllerParams;
struct FGameplayTag;
class UBaseUserWidget;
class UInventoryWidgetController;
class UCharacterWidgetController;
class UWidget;
class UDamageTextComponent;
class UBaseAbilitySystemComponent;
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
	
	UCharacterWidgetController* GetCharacterWidgetController(const FWidgetControllerParams& WCParams);
	UInventoryWidgetController* GetInventoryWidgetController(const FWidgetControllerParams& WCParams);
	UInventoryWidgetController* GetInventoryWidgetController() { return InventoryWidgetController; }
	APlayerCharacter* GetPlayerCharacter();

	UFUNCTION(BlueprintCallable)
	void SetCurrentlyHoveredButton(UBaseUserWidget* Button) { CurrentlyHoveredButton = Button; }

	UFUNCTION(BlueprintCallable)
	void HandleButtonPressed() const;

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
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="PlayerCharacter")
	TObjectPtr<UInputAction> ExecuteAction_LMB;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="PlayerCharacter")
	TObjectPtr<UInputAction> ExecuteAction_RMB;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="PlayerCharacter")
	TObjectPtr<UInputAction> ExecuteAction_1;
	
protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	
	void Move(const FInputActionValue& Value);
	void InteractButtonPressed(const FInputActionValue& Value);
	void Jump();
	void StopJumping();
	void InitializeRootLayout();

	/*
	 * Inventory
	 */
	virtual void UpdateInventorySlot_Implementation(EContainerType ContainerType, int32 SlotIndex, FInventoryItemData ItemData) override {}
	virtual void UpdateContainerSlots_Implementation(int32 TotalSlots) override {}
	UFUNCTION(BlueprintCallable) virtual void ToggleContainer_Implementation(bool bOpen, int32 SlotCount = 0) override;
	UFUNCTION(Client, Reliable) void InventoryButtonPressed();
	UFUNCTION(Client, Reliable) void ToggleInventory(bool bOpen);
	virtual void CloseInventory_Implementation() override;
	void ToggleInputAndMappingContext(bool bOpen);
	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName="ToggleInventory")) void K2_ToggleInventory(bool bOpen);
	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName="ToggleContainer")) void K2_ToggleContainer(bool bOpen);
	UPROPERTY(BlueprintReadWrite) bool bIsInventoryOpen = false;
	UPROPERTY(BlueprintReadWrite) bool bIsContainerOpen = false;
	
	UPROPERTY()
	TObjectPtr<UInventoryWidgetController> InventoryWidgetController;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UInventoryWidgetController> InventoryWidgetControllerClass;
	
	UPROPERTY()
	TObjectPtr<UCharacterWidgetController> CharacterWidgetController;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UCharacterWidgetController> CharacterWidgetControllerClass;
	
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UCommonUserWidget> MainLayout;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UCommonUserWidget> RootLayoutClass;
	
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

	TObjectPtr<UBaseUserWidget> CurrentlyHoveredButton;
	
	void CursorTrace();
	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);
	void AbilityInputTagCombo(FGameplayTag InputTag);
	UBaseAbilitySystemComponent* GetASC();
};
