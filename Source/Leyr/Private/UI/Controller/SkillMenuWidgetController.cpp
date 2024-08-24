// @ Retropsis 2024-2025.

#include "UI/Controller/SkillMenuWidgetController.h"
#include "GameplayTagContainer.h"
#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "Game/BaseGameplayTags.h"
#include "Player/PlayerCharacterState.h"

void USkillMenuWidgetController::BroadcastInitialValues()
{
	BroadcastAbilityInfo();
	SkillPointsChanged.Broadcast(GetBasePS()->GetSkillPoints());
}

void USkillMenuWidgetController::BindCallbacksToDependencies()
{
	GetBaseASC()->AbilityStatusChanged.AddLambda([this](const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, int32 NewLevel)
	{
		if (SelectedAbility.Ability.MatchesTagExact(AbilityTag))
		{
			SelectedAbility.Status = StatusTag;
			bool bEnableSpendPoints = false;
			bool bEnableEquip = false;
			ShouldEnableButtons(StatusTag, CurrentSpellPoints, bEnableSpendPoints, bEnableEquip);
			SkillSlotSelectedDelegate.Broadcast(bEnableSpendPoints, bEnableEquip);
		}
		if (AbilityInfo)
		{
			FBaseAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
			Info.StatusTag = StatusTag;
			AbilityInfoDelegate.Broadcast(Info);
		}
	});
	GetBasePS()->OnSkillPointsChangedDelegate.AddLambda([this](int32 SkillPoints)
	{
		SkillPointsChanged.Broadcast(SkillPoints);
		
		CurrentSpellPoints = SkillPoints;
		bool bEnableSpendPoints = false;
		bool bEnableEquip = false;
		ShouldEnableButtons(SelectedAbility.Status, CurrentSpellPoints, bEnableSpendPoints, bEnableEquip);
		SkillSlotSelectedDelegate.Broadcast(bEnableSpendPoints, bEnableEquip);
	});
}

void USkillMenuWidgetController::SkillSlotSelected(const FGameplayTag& AbilityTag)
{
	const FBaseGameplayTags GameplayTags = FBaseGameplayTags::Get();	
	const int32 SpellPoints = GetBasePS()->GetSkillPoints();
	FGameplayTag AbilityStatus;	

	const bool bTagValid = AbilityTag.IsValid();
	const bool bTagNone = AbilityTag.MatchesTag(GameplayTags.Abilities_None);
	const FGameplayAbilitySpec* AbilitySpec = GetBaseASC()->GetSpecFromAbilityTag(AbilityTag);
	const bool bSpecValid = AbilitySpec != nullptr;
	if (!bTagValid || bTagNone || !bSpecValid)
	{
		AbilityStatus = GameplayTags.Abilities_Status_Locked;
	}
	else
	{
		AbilityStatus = GetBaseASC()->GetStatusFromSpec(*AbilitySpec);
	}
	
	SelectedAbility.Ability = AbilityTag;
	SelectedAbility.Status = AbilityStatus;
	bool bEnableSpendPoints = false;
	bool bEnableEquip = false;
	ShouldEnableButtons(AbilityStatus, SpellPoints, bEnableSpendPoints, bEnableEquip);
	SkillSlotSelectedDelegate.Broadcast(bEnableSpendPoints, bEnableEquip);
}

void USkillMenuWidgetController::SpendPointButtonPressed()
{
	if (GetBaseASC()) GetBaseASC()->ServerSpendSkillPoint(SelectedAbility.Ability);
}

void USkillMenuWidgetController::ShouldEnableButtons(const FGameplayTag& AbilityStatus, int32 SkillPoints, bool& bShouldEnableSkillPointsButton, bool& bShouldEnableEquipButton)
{
	const FBaseGameplayTags GameplayTags = FBaseGameplayTags::Get();

	bShouldEnableSkillPointsButton = false;
	bShouldEnableEquipButton = false;
	if (AbilityStatus.MatchesTagExact(GameplayTags.Abilities_Status_Equipped))
	{
		bShouldEnableEquipButton = true;
		if (SkillPoints > 0)
		{
			bShouldEnableSkillPointsButton = true;
		}
	}
	else if (AbilityStatus.MatchesTagExact(GameplayTags.Abilities_Status_Eligible))
	{
		if (SkillPoints > 0)
		{
			bShouldEnableSkillPointsButton = true;
		}
	}
	else if (AbilityStatus.MatchesTagExact(GameplayTags.Abilities_Status_Unlocked))
	{
		bShouldEnableEquipButton = true;
		if (SkillPoints > 0)
		{
			bShouldEnableSkillPointsButton = true;
		}
	}
}
