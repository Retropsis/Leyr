// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "PlayerCharacterController.generated.h"

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
class LEYR_API APlayerCharacterController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void PlayerTick(float DeltaTime) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="PlayerCharacter")
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="PlayerCharacter")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="PlayerCharacter")
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="PlayerCharacter")
	TObjectPtr<UInputAction> AttackAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="PlayerCharacter")
	TObjectPtr<UInputAction> InteractAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="PlayerCharacter")
	TObjectPtr<UInputAction> EquipAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="PlayerCharacter")
	TObjectPtr<UInputAction> CrouchAction;
	
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
	void Jump();
	void StopJumping();
	void AttackButtonPressed();
	void CrouchButtonPressed();
	void InteractButtonPressed();
	
	/*
	 * Hotbar
	 */
	void HotbarButtonPressed(int32 Index);

private:
	UPROPERTY()
	TObjectPtr<APlayerCharacter> PlayerCharacter;
	
	void CursorTrace();
	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);

	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputConfig> InputConfig;

	UPROPERTY()
	TObjectPtr<UBaseAbilitySystemComponent> BaseAbilitySystemComponent;

	UBaseAbilitySystemComponent* GetASC();
};
