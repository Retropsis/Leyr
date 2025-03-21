// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "PlayerHUD.generated.h"

class UCommonActivatableWidget;
class USkillMenuWidgetController;
class UAttributeMenuWidgetController;
class UAttributeSet;
class UAbilitySystemComponent;
struct FWidgetControllerParams;
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
	UOverlayWidgetController* GetOverlayWidgetController(const FWidgetControllerParams& WCParams);
	UAttributeMenuWidgetController* GetAttributeMenuWidgetController(const FWidgetControllerParams& WCParams);
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
