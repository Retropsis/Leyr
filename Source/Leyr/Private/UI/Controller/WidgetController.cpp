// @ Retropsis 2024-2025.

#include "UI/Controller/WidgetController.h"
#include "AbilitySystemComponent.h"

void UWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& WCParams)
{
	PlayerController = WCParams.PlayerController;
	PlayerState = WCParams.PlayerState;
	AbilitySystemComponent = WCParams.AbilitySystemComponent;
	AttributeSet = WCParams.AttributeSet;

	AbilitySystemComponent->AbilityCommittedCallbacks.AddLambda([this] (UGameplayAbility* Ability)
	{
		OnAbilityCommitted(Ability);
	});
}
