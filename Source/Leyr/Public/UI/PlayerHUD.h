// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "PlayerHUD.generated.h"

struct FWidgetControllerParams;
class UCommonActivatableWidget;
class USkillMenuWidgetController;
class UAttributeMenuWidgetController;
class UAttributeSet;
class UAbilitySystemComponent;
class UOverlayWidgetController;
class UBaseUserWidget;

/**
 * 
 */
UCLASS()
class LEYR_API APlayerHUD : public AHUD
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	UOverlayWidgetController* GetOverlayWidgetController(const FWidgetControllerParams& WCParams);
	
	UFUNCTION(BlueprintCallable)
	UAttributeMenuWidgetController* GetAttributeMenuWidgetController(const FWidgetControllerParams& WCParams);
	
	UFUNCTION(BlueprintCallable)
	USkillMenuWidgetController* GetSkillMenuWidgetController(const FWidgetControllerParams& WCParams);
	
	void InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS);

	void PlayerDefeated();

	void ToggleInventory();

private:
	UPROPERTY()
	TObjectPtr<UBaseUserWidget>  OverlayWidget;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UBaseUserWidget> OverlayWidgetClass;

	UPROPERTY()
	TObjectPtr<UOverlayWidgetController> OverlayWidgetController;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UOverlayWidgetController> OverlayWidgetControllerClass;
	
	UPROPERTY()
	TObjectPtr<UAttributeMenuWidgetController> AttributeMenuWidgetController;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UAttributeMenuWidgetController> AttributeMenuWidgetControllerClass;

	UPROPERTY()
	TObjectPtr<USkillMenuWidgetController> SkillMenuWidgetController;

	UPROPERTY(EditAnywhere)
	TSubclassOf<USkillMenuWidgetController> SkillMenuWidgetControllerClass;

	/*
	 * Inventory
	 */
	UPROPERTY()
	TObjectPtr<UBaseUserWidget>  InventoryWidget;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UBaseUserWidget> InventoryWidgetClass;

	bool bIsInventoryOpen = false;

	/*
	 * Player Defeat
	*/
	UPROPERTY()
	TObjectPtr<UCommonActivatableWidget> PlayerDefeatWidget;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UCommonActivatableWidget>PlayerDefeatWidgetClass;
};
