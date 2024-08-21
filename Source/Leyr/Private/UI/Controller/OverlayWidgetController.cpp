// @ Retropsis 2024-2025.

#include "UI/Controller/OverlayWidgetController.h"

#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "AbilitySystem/BaseAttributeSet.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "AbilitySystem/Data/LevelUpInfo.h"
#include "Player/PlayerCharacterState.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	const UBaseAttributeSet* BaseAttributeSet = CastChecked<UBaseAttributeSet>(AttributeSet);
	
	OnHealthChanged.Broadcast(BaseAttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(BaseAttributeSet->GetMaxHealth());
	OnManaChanged.Broadcast(BaseAttributeSet->GetMana());
	OnMaxManaChanged.Broadcast(BaseAttributeSet->GetMaxMana());
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	APlayerCharacterState* PlayerCharacterState = CastChecked<APlayerCharacterState>(PlayerState);
	PlayerCharacterState->OnXPChangedDelegate.AddUObject(this, &UOverlayWidgetController::OnXPChanged);
	
	const UBaseAttributeSet* BaseAttributeSet = CastChecked<UBaseAttributeSet>(AttributeSet);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(BaseAttributeSet->GetHealthAttribute()).AddLambda(
		[this] (const FOnAttributeChangeData& Data)
		{
			OnHealthChanged.Broadcast(Data.NewValue);
		}
	);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(BaseAttributeSet->GetMaxHealthAttribute()).AddLambda(
		[this] (const FOnAttributeChangeData& Data)
		{
			OnMaxHealthChanged.Broadcast(Data.NewValue);
		}
	);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(BaseAttributeSet->GetManaAttribute()).AddLambda(
		[this] (const FOnAttributeChangeData& Data)
		{
			OnManaChanged.Broadcast(Data.NewValue);
		}
	);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(BaseAttributeSet->GetMaxManaAttribute()).AddLambda(
		[this] (const FOnAttributeChangeData& Data)
		{
			OnMaxManaChanged.Broadcast(Data.NewValue);
		}
	);

	if (UBaseAbilitySystemComponent* BaseASC = Cast<UBaseAbilitySystemComponent>(AbilitySystemComponent))
	{
		if (BaseASC->bStartupAbilitiesGiven)
		{
			OnInitializeStartupAbilities(BaseASC);
		}
		else
		{
			BaseASC->AbilitiesGivenDelegate.AddUObject(this, &UOverlayWidgetController::OnInitializeStartupAbilities);
		}
		BaseASC->EffectAssetTags.AddLambda([this] (const FGameplayTagContainer& AssetTags)
		{
			for (const FGameplayTag& Tag : AssetTags)
			{
				// For example, say that Tag = Message.HealthPotion
				// "Message.HealthPotion".MatchesTag("Message") will return True, "Message".MatchesTag("Message.HealthPotion") will return False
				FGameplayTag MessageTag = FGameplayTag::RequestGameplayTag(FName("Message"));
				if (Tag.MatchesTag(MessageTag))
				{
					const FUIWidgetRow* Row = GetDataTableRowByTag<FUIWidgetRow>(MessageWidgetDataTable, Tag);
					MessageWidgetRowDelegate.Broadcast(*Row);
				}
			}
		});
	}	
}

void UOverlayWidgetController::OnInitializeStartupAbilities(UBaseAbilitySystemComponent* BaseAbilitySystemComponent)
{
	if (!BaseAbilitySystemComponent->bStartupAbilitiesGiven) return;
	
	FForEachAbility BroadcastDelegate;
	BroadcastDelegate.BindLambda([this, BaseAbilitySystemComponent](const FGameplayAbilitySpec& AbilitySpec)
	{
		//TODO need a way to figure out the ability tag for a given ability spec.
		FBaseAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(BaseAbilitySystemComponent->GetAbilityTagFromSpec(AbilitySpec));
		Info.InputTag = BaseAbilitySystemComponent->GetInputTagFromSpec(AbilitySpec);
		AbilityInfoDelegate.Broadcast(Info);
	});
	BaseAbilitySystemComponent->ForEachAbility(BroadcastDelegate);
}

void UOverlayWidgetController::OnXPChanged(int32 NewXP) const
{
	const APlayerCharacterState* PlayerCharacterState = CastChecked<APlayerCharacterState>(PlayerState);
	const ULevelUpInfo* LevelUpInfo = PlayerCharacterState->LevelUpInfo;
	checkf(LevelUpInfo, TEXT("Unabled to find LevelUpInfo. Please fill out AuraPlayerState Blueprint"));

	const int32 Level = LevelUpInfo->FindLevelForXP(NewXP);
	const int32 MaxLevel = LevelUpInfo->LevelUpInformation.Num();

	if (Level <= MaxLevel && Level > 0)
	{
		const int32 LevelUpRequirement = LevelUpInfo->LevelUpInformation[Level].LevelUpRequirement;
		const int32 PreviousLevelUpRequirement = LevelUpInfo->LevelUpInformation[Level - 1].LevelUpRequirement;

		const int32 DeltaLevelRequirement = LevelUpRequirement - PreviousLevelUpRequirement;
		const int32 XPForThisLevel = NewXP - PreviousLevelUpRequirement;

		const float XPBarPercent = static_cast<float>(XPForThisLevel) / static_cast<float>(DeltaLevelRequirement);

		OnXPPercentChangedDelegate.Broadcast(XPBarPercent);
		OnXPValueChanged.Broadcast(XPForThisLevel);
	}
}
