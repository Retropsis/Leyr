// @ Retropsis 2024-2025.

#include "UI/Controller/CharacterWidgetController.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "AbilitySystem/BaseAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"
#include "AbilitySystem/Data/LevelUpInfo.h"
#include "Data/AbilityData.h"
#include "Player/PlayerCharacterState.h"
#include "UI/Controller/InventoryWidgetController.h"

void UCharacterWidgetController::BroadcastInitialValues()
{
	check(AttributeInfo);
	for (auto& Info : AttributeInfo.Get()->AttributeInformation)
	{
		BroadcastAttributeInfo(Info.AttributeTag);
	}
	OnHealthChanged.Broadcast(GetBaseAS()->GetHealth());
	OnMaxHealthChanged.Broadcast(GetBaseAS()->GetMaxHealth());
	OnManaChanged.Broadcast(GetBaseAS()->GetMana());
	OnMaxManaChanged.Broadcast(GetBaseAS()->GetMaxMana());
	OnPlayerLevelChanged.Broadcast(GetBasePS()->GetCharacterLevel());
	OnXPChanged(GetBasePS()->GetXP());
	AttributePointsChangedDelegate.Broadcast(GetBasePS()->GetAttributePoints());
}

void UCharacterWidgetController::BindCallbacksToDependencies()
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
	GetBasePS()->OnXPChangedDelegate.AddUObject(this, &UCharacterWidgetController::OnXPChanged);
	GetBasePS()->OnLevelChangedDelegate.AddLambda([this](int32 NewLevel, bool bLevelUp)
		{
			OnPlayerLevelChanged.Broadcast(NewLevel);
		}
	);
	GetBasePS()->OnAttributePointsChangedDelegate.AddLambda([this](int32 NewCount)
	{
		AttributePointsChangedDelegate.Broadcast(NewCount);
	});
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetBaseAS()->GetHealthAttribute()).AddLambda(
		[this] (const FOnAttributeChangeData& Data)
		{
			OnHealthChanged.Broadcast(Data.NewValue);
		}
	);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetBaseAS()->GetMaxHealthAttribute()).AddLambda(
		[this] (const FOnAttributeChangeData& Data)
		{
			OnMaxHealthChanged.Broadcast(Data.NewValue);
		}
	);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetBaseAS()->GetManaAttribute()).AddLambda(
		[this] (const FOnAttributeChangeData& Data)
		{
			OnManaChanged.Broadcast(Data.NewValue);
		}
	);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetBaseAS()->GetMaxManaAttribute()).AddLambda(
		[this] (const FOnAttributeChangeData& Data)
		{
			OnMaxManaChanged.Broadcast(Data.NewValue);
		}
	);
}

void UCharacterWidgetController::UpgradeAttribute(const FGameplayTag& AttributeTag)
{
	GetBaseASC()->UpgradeAttribute(AttributeTag);
}

void UCharacterWidgetController::OnXPChanged(int32 NewXP)
{
	const ULevelUpInfo* LevelUpInfo = GetBasePS()->LevelUpInfo;
	checkf(LevelUpInfo, TEXT("Unabled to find LevelUpInfo. Please fill out BasePlayerState Blueprint"));

	const int32 Level = LevelUpInfo->FindLevelForXP(NewXP);
	const int32 MaxLevel = LevelUpInfo->LevelUpInformation.Num();

	if (Level <= MaxLevel && Level > 0)
	{
		const int32 LevelUpRequirement = LevelUpInfo->LevelUpInformation[Level].LevelUpRequirement;
		OnXPToNextLevelChanged.Broadcast(LevelUpRequirement - NewXP);
		OnXPValueChanged.Broadcast(NewXP);
	}
}

void UCharacterWidgetController::BroadcastAttributeInfo(const FGameplayTag& AttributeTag) const
{
	FBaseAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(AttributeTag);
	Info.AttributeValue = Info.AttributeGetter.GetNumericValue(AttributeSet);
	Info.AttributeBaseValue = Info.AttributeGetter.GetGameplayAttributeData(AttributeSet)->GetBaseValue();
	AttributeInfoDelegate.Broadcast(Info);
}
