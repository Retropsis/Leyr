// @ Retropsis 2024-2025.

#include "UI/Controller/WidgetController.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "AbilitySystem/BaseAttributeSet.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "Player/PlayerCharacterController.h"
#include "Player/PlayerCharacterState.h"

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

void UWidgetController::BroadcastAbilityInfo()
{
	if (!GetBaseASC()->bStartupAbilitiesGiven) return;

	FForEachAbility BroadcastDelegate;
	BroadcastDelegate.BindLambda([this](const FGameplayAbilitySpec& AbilitySpec)
	{
		FBaseAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(BaseAbilitySystemComponent->GetAbilityTagFromSpec(AbilitySpec));
		Info.InputTag = BaseAbilitySystemComponent->GetInputTagFromSpec(AbilitySpec);
		Info.StatusTag = BaseAbilitySystemComponent->GetStatusFromSpec(AbilitySpec);
		AbilityInfoDelegate.Broadcast(Info);
	});
	GetBaseASC()->ForEachAbility(BroadcastDelegate);
}

APlayerCharacterController* UWidgetController::GetBasePC()
{
	if (PlayerCharacterController == nullptr)
	{
		PlayerCharacterController = Cast<APlayerCharacterController>(PlayerController);
	}
	return PlayerCharacterController;
}

APlayerCharacterState* UWidgetController::GetBasePS()
{
	if (PlayerCharacterState == nullptr)
	{
		PlayerCharacterState = Cast<APlayerCharacterState>(PlayerState);
	}
	return PlayerCharacterState;
}

UBaseAbilitySystemComponent* UWidgetController::GetBaseASC()
{
	if (BaseAbilitySystemComponent == nullptr)
	{
		BaseAbilitySystemComponent = Cast<UBaseAbilitySystemComponent>(AbilitySystemComponent);
	}
	return BaseAbilitySystemComponent;
}

UBaseAttributeSet* UWidgetController::GetBaseAS()
{
	if (BaseAttributeSet == nullptr)
	{
		BaseAttributeSet = Cast<UBaseAttributeSet>(AttributeSet);
	}
	return BaseAttributeSet;
}
