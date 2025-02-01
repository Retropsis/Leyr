// @ Retropsis 2024-2025.

#include "UI/Controller/AttributeMenuWidgetController.h"
#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "AbilitySystem/BaseAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"
#include "Player/PlayerCharacterState.h"

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	check(AttributeInfo);
	for (auto& Info : AttributeInfo.Get()->AttributeInformation)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Info.AttributeGetter).AddLambda(
		[this, Info] (const FOnAttributeChangeData& Data)
			{
				BroadcastAttributeInfo(Info.AttributeTag);
			}
		);
	}
	GetBasePS()->OnAttributePointsChangedDelegate.AddLambda(
		[this] (int32 Points)
		{
			AttributePointsChangedDelegate.Broadcast(Points);
		}
	);
}

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	check(AttributeInfo);
	for (auto& Info : AttributeInfo.Get()->AttributeInformation)
	{
		BroadcastAttributeInfo(Info.AttributeTag);
	}
	AttributePointsChangedDelegate.Broadcast(GetBasePS()->GetAttributePoints());
}

void UAttributeMenuWidgetController::UpgradeAttribute(const FGameplayTag& AttributeTag)
{
	UBaseAbilitySystemComponent* BaseASC = CastChecked<UBaseAbilitySystemComponent>(AbilitySystemComponent);
	BaseASC->UpgradeAttribute(AttributeTag);
}

void UAttributeMenuWidgetController::BroadcastAttributeInfo(const FGameplayTag& AttributeTag) const
{
	FBaseAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(AttributeTag);
	Info.AttributeValue = Info.AttributeGetter.GetNumericValue(AttributeSet);
	Info.AttributeBaseValue = Info.AttributeGetter.GetGameplayAttributeData(AttributeSet)->GetBaseValue();
	AttributeInfoDelegate.Broadcast(Info);
}
