// @ Retropsis 2024-2025.

#include "UI/PlayerHUD.h"

#include "UI/Controller/AttributeMenuWidgetController.h"
#include "UI/Controller/OverlayWidgetController.h"
#include "UI/Controller/SkillMenuWidgetController.h"
#include "UI/Widget/BaseUserWidget.h"

UOverlayWidgetController* APlayerHUD::GetOverlayWidgetController(const FWidgetControllerParams& WCParams)
{
	if(OverlayWidgetController == nullptr)
	{
		OverlayWidgetController = NewObject<UOverlayWidgetController>(this, OverlayWidgetControllerClass);
		OverlayWidgetController->SetWidgetControllerParams(WCParams);
		OverlayWidgetController->BindCallbacksToDependencies();
	}
	return OverlayWidgetController;
}

UAttributeMenuWidgetController* APlayerHUD::GetAttributeMenuWidgetController(const FWidgetControllerParams& WCParams)
{
	if (AttributeMenuWidgetController == nullptr)
	{
		AttributeMenuWidgetController = NewObject<UAttributeMenuWidgetController>(this, AttributeMenuWidgetControllerClass);
		AttributeMenuWidgetController->SetWidgetControllerParams(WCParams);
		AttributeMenuWidgetController->BindCallbacksToDependencies();
	}
	return AttributeMenuWidgetController;
}

USkillMenuWidgetController* APlayerHUD::GetSkillMenuWidgetController(const FWidgetControllerParams& WCParams)
{
	if (SkillMenuWidgetController == nullptr)
	{
		SkillMenuWidgetController = NewObject<USkillMenuWidgetController>(this, SkillMenuWidgetControllerClass);
		SkillMenuWidgetController->SetWidgetControllerParams(WCParams);
		SkillMenuWidgetController->BindCallbacksToDependencies();
	}
	return SkillMenuWidgetController;
}

void APlayerHUD::InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	checkf(OverlayWidgetClass, TEXT("Overlay Widget Class uninitialized, please fill out BP_PlayerHUD"));
	checkf(OverlayWidgetControllerClass, TEXT("Overlay Widget Controller Class uninitialized, please fill out BP_PlayerHUD"));

	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass);
	OverlayWidget = Cast<UBaseUserWidget>(Widget);

	const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
	UOverlayWidgetController* WidgetController = GetOverlayWidgetController(WidgetControllerParams);

	OverlayWidget->SetWidgetController(WidgetController);
	WidgetController->BroadcastInitialValues();
	
	Widget->AddToViewport();
}

void APlayerHUD::ToggleInventory()
{
	if(!IsValid(InventoryWidget))
	{
		InventoryWidget = CreateWidget<UBaseUserWidget>(GetOwningPlayerController(), InventoryWidgetClass);
	}
	if(IsValid(InventoryWidget))
	{
		if(InventoryWidget->IsInViewport())
		{
			InventoryWidget->RemoveFromParent();
			const FInputModeGameOnly GameOnlyMode;
			GetOwningPlayerController()->SetInputMode(GameOnlyMode);
			GetOwningPlayerController()->SetShowMouseCursor(false);
		}
		else
		{
			InventoryWidget->AddToViewport();
			const FInputModeGameAndUI GameAndUIMode;
			GetOwningPlayerController()->SetInputMode(GameAndUIMode);
			GetOwningPlayerController()->SetShowMouseCursor(true);
		}
	}
}
