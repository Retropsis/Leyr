// @ Retropsis 2024-2025.

#include "UI/Controller/SkillMenuWidgetController.h"
#include "GameplayTagContainer.h"
#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "AbilitySystem/Data/PassiveInfo.h"
#include "Game/BaseGameplayTags.h"
#include "GameplayEffectComponents/TargetTagsGameplayEffectComponent.h"
#include "Interaction/PlayerInterface.h"
#include "Player/PlayerCharacterState.h"

void USkillMenuWidgetController::BroadcastInitialValues()
{
	BroadcastAbilityInfo();
	// BaseAbilitySystemComponent->UpdateAbilityStatuses(PlayerCharacterState->GetCharacterLevel());
	SkillPointsChanged.Broadcast(GetBasePS()->GetSkillPoints());
	OnPlayerLevelChanged.Broadcast(GetBasePS()->GetCharacterLevel());
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
			FString Description;
			FString NextLevelDescription;
			GetBaseASC()->GetDescriptionsByAbilityTag(AbilityTag, Description, NextLevelDescription);
			SkillSlotSelectedDelegate.Broadcast(bEnableSpendPoints, bEnableEquip, Description, NextLevelDescription);
		}
		if (AbilityInfo)
		{
			FBaseAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
			Info.StatusTag = StatusTag;
			AbilityInfoDelegate.Broadcast(Info);
		}
	});
	GetBaseASC()->AbilityEquipped.AddUObject(this, &USkillMenuWidgetController::OnAbilityEquipped);
	
	GetBasePS()->OnSkillPointsChangedDelegate.AddLambda([this](int32 SkillPoints)
	{
		SkillPointsChanged.Broadcast(SkillPoints);
		
		CurrentSpellPoints = SkillPoints;
		bool bEnableSpendPoints = false;
		bool bEnableEquip = false;
		ShouldEnableButtons(SelectedAbility.Status, CurrentSpellPoints, bEnableSpendPoints, bEnableEquip);
		FString Description;
		FString NextLevelDescription;
		GetBaseASC()->GetDescriptionsByAbilityTag(SelectedAbility.Ability, Description, NextLevelDescription);
		SkillSlotSelectedDelegate.Broadcast(bEnableSpendPoints, bEnableEquip, Description, NextLevelDescription);
	});
	
	const FBaseGameplayTags& GameplayTags = FBaseGameplayTags::Get();
	EquippedAbilities.FindOrAdd(GameplayTags.Abilities_Type_ActionSlot_Main, FEquippedAbility{});
	EquippedAbilities.FindOrAdd(GameplayTags.Abilities_Type_ActionSlot_Sub, FEquippedAbility{});
	EquippedAbilities.FindOrAdd(GameplayTags.Abilities_Type_PassiveSlot, FEquippedAbility{});
}

void USkillMenuWidgetController::SkillSlotSelected(const FGameplayTag& AbilityTag)
{
	if (bWaitingForEquipSelection)
	{
		const FGameplayTag SelectedAbilityType = AbilityInfo->FindAbilityInfoForTag(SelectedAbility.Ability).AbilityType;
		StopWaitingForEquipDelegate.Broadcast(SelectedAbilityType);
		bWaitingForEquipSelection = false;
	}
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
	FString Description;
	FString NextLevelDescription;
	GetBaseASC()->GetDescriptionsByAbilityTag(AbilityTag, Description, NextLevelDescription);
	SkillSlotSelectedDelegate.Broadcast(bEnableSpendPoints, bEnableEquip, Description, NextLevelDescription);
}

void USkillMenuWidgetController::SkillSlotDeselect()
{
	if (bWaitingForEquipSelection)
	{
		const FGameplayTag SelectedAbilityType = AbilityInfo->FindAbilityInfoForTag(SelectedAbility.Ability).AbilityType;
		StopWaitingForEquipDelegate.Broadcast(SelectedAbilityType);
		bWaitingForEquipSelection = false;
	}
	SelectedAbility.Ability = FBaseGameplayTags::Get().Abilities_None;
	SelectedAbility.Status = FBaseGameplayTags::Get().Abilities_Status_Locked;

	SkillSlotSelectedDelegate.Broadcast(false, false, FString(), FString());
}

void USkillMenuWidgetController::EquipButtonPressed()
{
	const FGameplayTag AbilityType = AbilityInfo->FindAbilityInfoForTag(SelectedAbility.Ability).AbilityType;

	WaitForEquipDelegate.Broadcast(AbilityType);
	bWaitingForEquipSelection = true;
	
	const FGameplayTag SelectedStatus = GetBaseASC()->GetStatusFromAbilityTag(SelectedAbility.Ability);
	if (SelectedStatus.MatchesTagExact(FBaseGameplayTags::Get().Abilities_Status_Equipped))
	{
		SelectedSlot = GetBaseASC()->GetInputTagFromAbilityTag(SelectedAbility.Ability);
	}
}

void USkillMenuWidgetController::ActivateButtonPressed(const FGameplayTag& AbilityTag)
{
	const FBasePassiveInfo Passive = PassiveInfo->FindPassiveInfoForTag(AbilityTag);

	if (Passive.Modifiers.IsEmpty()) return;

	ActivatedEffects.Add(AbilityTag, FPassiveEffect{ Passive.Modifiers });
	
	if(ActiveEquipmentEffectHandle.IsValid()) AbilitySystemComponent->RemoveActiveGameplayEffect(ActiveEquipmentEffectHandle);
	
	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	FString SourceObjectName = FString::Printf(TEXT("GE_PassiveEffects"));
	UGameplayEffect* Effect = NewObject<UGameplayEffect>(this, FName(SourceObjectName));

	Effect->DurationPolicy = EGameplayEffectDurationType::Infinite;
	
	UTargetTagsGameplayEffectComponent& AssetTagsComponent = Effect->FindOrAddComponent<UTargetTagsGameplayEffectComponent>();
	FInheritedTagContainer InheritedTagContainer;

	for (const TTuple<FGameplayTag, FPassiveEffect> ActivatedEffect : ActivatedEffects)
	{
		Effect->Modifiers.Append(ActivatedEffect.Value.Modifiers);
		InheritedTagContainer.Added.AddTag(ActivatedEffect.Key);
	}
	AssetTagsComponent.SetAndApplyTargetTagChanges(InheritedTagContainer);

	if (FGameplayEffectSpec* MutableSpec = new FGameplayEffectSpec(Effect, EffectContext, 1.f))
	{
		ActiveEquipmentEffectHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*MutableSpec, AbilitySystemComponent);
	}
}

TArray<FBaseAbilityInfo> USkillMenuWidgetController::GetAvailableAbilities(const FGameplayTagContainer& Filters)
{
	TArray<FBaseAbilityInfo> Abilities;

	if (AbilityInfo)
	{
		for (FBaseAbilityInfo& Info : AbilityInfo->AbilityInformation)
		{
			if (Filters.IsEmpty() || Filters.HasTag(Info.AbilityType))
			{
				Abilities.Add(Info);
			}
		}
	}
	return Abilities;
}

TMap<FGameplayTag, FBaseAbilityInfo> USkillMenuWidgetController::GetEquippedAbilityInfos()
{
	TMap<FGameplayTag, FBaseAbilityInfo> Infos;

	for (TTuple<FGameplayTag, FEquippedAbility> EquippedAbility :  EquippedAbilities)
	{		
		Infos.Add(EquippedAbility.Key, AbilityInfo->FindAbilityInfoForTag(EquippedAbility.Value.AbilityTag));
	}
	return Infos;
}

void USkillMenuWidgetController::AbilityRowButtonPressed(const FBaseAbilityInfo& AbilityToEquipInfo)
{
	FGameplayAbilitySpec* AbilitySpec = GetBaseASC()->GetSpecFromAbilityTag(AbilityToEquipInfo.AbilityTag);
	if (AbilitySpec == nullptr) return;
	if (AbilitySpec && !AbilitySpec->Handle.IsValid()) return;
	
	const FBaseGameplayTags& GameplayTags = FBaseGameplayTags::Get();
	const FGameplayTag InputTag = GameplayTags.AbilitySlotToInputTags[AbilityToEquipInfo.AbilityType]; 
	
	// Active Slot
	if (AbilityToEquipInfo.AbilityType.MatchesTag(GameplayTags.Abilities_Type_ActionSlot))
	{
		AbilitySpec->DynamicAbilityTags.Reset();
		FEquippedAbility* FoundAbility = EquippedAbilities.Find(AbilityToEquipInfo.AbilityType);
		if (FoundAbility && FoundAbility->AbilityTag.IsValid() && AbilityToEquipInfo.AbilityTag.MatchesTagExact(FoundAbility->AbilityTag))
		{
			// Deactivate Ability
			EquippedAbilities.Add(AbilityToEquipInfo.AbilityType, FEquippedAbility{});
			GetBaseASC()->ClientEquipAbility(AbilityToEquipInfo.AbilityTag, GameplayTags.Abilities_Status_Eligible, InputTag, FGameplayTag());
		}
		else
		{
			// Activate Ability
			EquippedAbilities.Add(AbilityToEquipInfo.AbilityType, FEquippedAbility{ AbilityToEquipInfo.AbilityTag });
			AbilitySpec->DynamicAbilityTags.AppendTags(InputTag.GetSingleTagContainer());
			GetBaseASC()->ClientEquipAbility(AbilityToEquipInfo.AbilityTag, GameplayTags.Abilities_Status_Equipped, InputTag, FGameplayTag());
		}
	}
	
	// Passive Ability Slot
	if (AbilityToEquipInfo.AbilityType.MatchesTagExact(GameplayTags.Abilities_Type_PassiveSlot))
	{
		FEquippedAbility* FoundAbility = EquippedAbilities.Find(AbilityToEquipInfo.AbilityType);
		if (FoundAbility && FoundAbility->AbilityTag.IsValid() && AbilityToEquipInfo.AbilityTag.MatchesTagExact(FoundAbility->AbilityTag))
		{
			// Deactivate passive
			EquippedAbilities.Add(AbilityToEquipInfo.AbilityType, FEquippedAbility{});
			if (FGameplayAbilitySpec* FoundAbilitySpec = AbilitySystemComponent->FindAbilitySpecFromClass(AbilityToEquipInfo.Ability))
			{
				AbilitySystemComponent->CancelAbilityHandle(FoundAbilitySpec->Handle);
			}
			GetBaseASC()->ClientEquipAbility(AbilityToEquipInfo.AbilityTag, GameplayTags.Abilities_Status_Eligible, InputTag, FGameplayTag());
		}
		else
		{
			// Activate passive
			EquippedAbilities.Add(AbilityToEquipInfo.AbilityType, FEquippedAbility{ AbilityToEquipInfo.AbilityTag });
			AbilitySystemComponent->TryActivateAbilityByClass(AbilityToEquipInfo.Ability);
			GetBaseASC()->ClientEquipAbility(AbilityToEquipInfo.AbilityTag, GameplayTags.Abilities_Status_Equipped, InputTag, FGameplayTag());
		}
	}
}

void USkillMenuWidgetController::GetDescription(const FGameplayTag& AbilityTag)
{
	FString Description;
	FString NextLevelDescription;
	GetBaseASC()->GetDescriptionsByAbilityTag(AbilityTag, Description, NextLevelDescription);
	OnDescriptionUpdated.Broadcast(Description, NextLevelDescription);
}

void USkillMenuWidgetController::SkillRowSlotPressed(const FGameplayTag& SlotTag, const FGameplayTag& AbilityType)
{
	if (!bWaitingForEquipSelection) return;
	// Check selected ability against the slot's ability type.
	// (don't equip an offensive spell in a passive slot and vice versa)
	const FGameplayTag& SelectedAbilityType = AbilityInfo->FindAbilityInfoForTag(SelectedAbility.Ability).AbilityType;
	if (!SelectedAbilityType.MatchesTagExact(AbilityType)) return;

	GetBaseASC()->ServerEquipAbility(SelectedAbility.Ability, SlotTag);
}

void USkillMenuWidgetController::OnAbilityEquipped(const FGameplayTag& AbilityTag, const FGameplayTag& Status, const FGameplayTag& Slot, const FGameplayTag& PreviousSlot)
{
	bWaitingForEquipSelection = false;

	const FBaseGameplayTags& GameplayTags = FBaseGameplayTags::Get();

	FBaseAbilityInfo LastSlotInfo;
	LastSlotInfo.StatusTag = GameplayTags.Abilities_Status_Unlocked;
	LastSlotInfo.InputTag = PreviousSlot;
	LastSlotInfo.AbilityTag = GameplayTags.Abilities_None;
	// Broadcast empty info if PreviousSlot is a valid slot. Only if equipping an already-equipped spell
	AbilityInfoDelegate.Broadcast(LastSlotInfo);

	FBaseAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
	Info.StatusTag = Status;
	Info.InputTag = Slot;
	AbilityInfoDelegate.Broadcast(Info);

	StopWaitingForEquipDelegate.Broadcast(AbilityInfo->FindAbilityInfoForTag(AbilityTag).AbilityType);
	SkillSlotReassignedDelegate.Broadcast(AbilityTag);
	SkillSlotDeselect();
}

bool USkillMenuWidgetController::SpendPointButtonPressed()
{
	if (GetBaseASC())
	{
		if (GetBaseASC()->GetAvatarActor()->Implements<UPlayerInterface>())
		{
			if (IPlayerInterface::Execute_GetSkillPoints(GetBaseASC()->GetAvatarActor()) > 0)
			{
				GetBaseASC()->ServerSpendSkillPoint(SelectedAbility.Ability);
				return true;
			}
		}
	}
	return false;
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
