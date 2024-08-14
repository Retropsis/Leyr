// @ Retropsis 2024-2025.

#include "UI/Controller/AttributeMenuWidgetController.h"
#include "AbilitySystem/BaseAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	check(AttributeInfo);
	
	for (auto& Info : AttributeInfo.Get()->AttributeInformation)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Info.AttributeGetter).AddLambda(
		[this, Info](const FOnAttributeChangeData& Data)
			{
				BroadcastAttributeInfo(Info.AttributeTag);
			}
		);
	}
}

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	UBaseAttributeSet* AS = CastChecked<UBaseAttributeSet>(AttributeSet);

	check(AttributeInfo);

	for (auto& Info : AttributeInfo.Get()->AttributeInformation)
	{
		BroadcastAttributeInfo(Info.AttributeTag);
	}
	// for (auto& Pair : AS->TagsToAttributes)
	// {
	// 	FBaseAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(Pair.Key);
	// 	Info.AttributeValue = Pair.Value().GetNumericValue(AS);
	// 	AttributeInfoDelegate.Broadcast(Info);
	// }
}

void UAttributeMenuWidgetController::BroadcastAttributeInfo(const FGameplayTag& AttributeTag) const
{
	FBaseAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(AttributeTag);
	Info.AttributeValue = Info.AttributeGetter.GetNumericValue(AttributeSet);
	AttributeInfoDelegate.Broadcast(Info);
}
