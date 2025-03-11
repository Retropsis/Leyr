// @ Retropsis 2024-2025.

#include "AbilitySystem/Ability/BaseGameplayAbility.h"
#include "PaperZDAnimInstance.h"
#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "AbilitySystem/BaseAttributeSet.h"
#include "AbilitySystem/LeyrAbilitySystemLibrary.h"
#include "AI/BaseCharacter.h"
#include "Data/AbilityData.h"
#include "Data/ItemData.h"
#include "Game/BaseGameplayTags.h"
#include "GameplayEffectComponents/TargetTagsGameplayEffectComponent.h"
#include "Interaction/CombatInterface.h"
#include "Interaction/PlayerInterface.h"

void UBaseGameplayAbility::InitAbility()
{
	if (!GetAvatarActorFromActorInfo()->Implements<UCombatInterface>()) return;
	
	PaperAnimInstance = ICombatInterface::Execute_GetPaperAnimInstance(GetAvatarActorFromActorInfo());
	WeaponAnimInstance = ICombatInterface::Execute_GetWeaponAnimInstance(GetAvatarActorFromActorInfo());
	WeaponAnimInstance->StopAllAnimationOverrides();
	
	DamageType = FBaseGameplayTags::Get().Damage_Physical;
	
	/*
	 * Initialize from Default Ability Data asset
	 */
	if (AbilityData)
	{
		// AbilityPower = AbilityData->AbilityPower;
		AbilityPoise = AbilityData->AbilityPoise;
		SequenceType = AbilityData->SequenceType;
		DamageType = AbilityData->DamageType;
	} 
	else if (AbilityData = ICombatInterface::Execute_LoadAndGetDefaultAbilityData(GetAvatarActorFromActorInfo()); AbilityData)
	{
		SequenceType = AbilityData->SequenceType;
		DamageType = AbilityData->DamageType;
	}

	/*
	 * Initialize from Item Data asset
	 */
	if (AbilityItemData = Cast<UItemData>(GetSourceObjectFromAbilitySpec()); AbilityItemData)
	{
		if (AbilityItemData->AnimationInstance)
		{
			if (const ABaseCharacter* Character = Cast<ABaseCharacter>(GetAvatarActorFromActorInfo()))
			{
				WeaponAnimInstance = Character->SetWeaponAnimInstance(AbilityItemData->AnimationInstance);
			}
		}
		SequenceType = AbilityItemData->SequenceType;
		DamageType = AbilityItemData->DamageType;
	}
	
	const float PoiseChance = GetAbilitySystemComponentFromActorInfo()->GetNumericAttribute(UBaseAttributeSet::GetPoiseAttribute());
	const float EffectivePoiseChance = PoiseChance + AbilityPoise;

	if(FMath::RandRange(1, 100) < EffectivePoiseChance)
	{
		GetAbilitySystemComponentFromActorInfo()->AddLooseGameplayTag(FBaseGameplayTags::Get().Poise);
		bPoiseWasApplied = true;
	}
}

void UBaseGameplayAbility::PrepareToEndAbility()
{
	if(bPoiseWasApplied)
	{
		GetAbilitySystemComponentFromActorInfo()->RemoveLooseGameplayTag(FBaseGameplayTags::Get().Poise);
	}
}

void UBaseGameplayAbility::MakeAndApplyExecuteEffectToTarget(const FGameplayTag& TagToApply, UAbilitySystemComponent* TargetASC, int32 Level)
{
	const FBaseGameplayTags& GameplayTags = FBaseGameplayTags::Get();
	FGameplayEffectContextHandle EffectContext = GetAbilitySystemComponentFromActorInfo()->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	FString TagName = FString::Printf(TEXT("%s"), *TagToApply.ToString());
	UGameplayEffect* Effect = NewObject<UGameplayEffect>(GetTransientPackage(), FName(TagName));

	Effect->DurationPolicy = EGameplayEffectDurationType::HasDuration;
	Effect->DurationMagnitude = FGameplayEffectModifierMagnitude{ 1.75f };
	Effect->StackingType = EGameplayEffectStackingType::AggregateByTarget;
	Effect->StackLimitCount = 6;
	Effect->StackExpirationPolicy = EGameplayEffectStackingExpirationPolicy::RemoveSingleStackAndRefreshDuration;
	
	UTargetTagsGameplayEffectComponent& AssetTagsComponent = Effect->FindOrAddComponent<UTargetTagsGameplayEffectComponent>();
	FInheritedTagContainer InheritedTagContainer;
	InheritedTagContainer.Added.AddTag(TagToApply);
	AssetTagsComponent.SetAndApplyTargetTagChanges(InheritedTagContainer);

	// const int32 Index = Effect->Modifiers.Num();
	// Effect->Modifiers.Add(FGameplayModifierInfo());
	// FGameplayModifierInfo& ModifierInfo = Effect->Modifiers[Index];
	//
	// ModifierInfo.ModifierMagnitude = FScalableFloat(StatusEffectDamage);
	// ModifierInfo.ModifierOp = EGameplayModOp::Additive;
	// ModifierInfo.Attribute = UBaseAttributeSet::GetIncomingDamageAttribute();

	if (FGameplayEffectSpec* MutableSpec = new FGameplayEffectSpec(Effect, EffectContext, Level))
	{
		// FBaseGameplayEffectContext* BaseContext = static_cast<FBaseGameplayEffectContext*>(MutableSpec->GetContext().Get());
		// TSharedPtr<FGameplayTag> StatusEffectDamageType = MakeShareable(new FGameplayTag(DamageType));
		// BaseContext->SetDamageType(StatusEffectDamageType);

		const FActiveGameplayEffectHandle Handle = GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(*MutableSpec, TargetASC);
		if (Handle.IsValid()) TargetASC->OnGameplayEffectAppliedToTarget(TargetASC, *MutableSpec, Handle);
	}
}

UObject* UBaseGameplayAbility::GetSourceObjectFromAbilitySpec()
{
	FGameplayAbilitySpec* AbilitySpec = GetCurrentAbilitySpec();
	return AbilitySpec->SourceObject.Get();
}

bool UBaseGameplayAbility::CommitInventoryCost(bool bIsSelfCost)
{
	const FGameplayAbilitySpec* AbilitySpec = GetCurrentAbilitySpec();
	if(UItemData* ItemData = Cast<UItemData>(AbilitySpec->SourceObject.Get()))
	{
		int32 OutQuantity;
		const bool bWasSuccessful = IPlayerInterface::Execute_UseItem(GetAvatarActorFromActorInfo(), ItemData, 1, bIsSelfCost, OutQuantity);
		if (UBaseAbilitySystemComponent* BaseASC = Cast<UBaseAbilitySystemComponent>(GetAbilitySystemComponentFromActorInfo()); bWasSuccessful && BaseASC)
		{
			BaseASC->AbilityCostCommitted.Broadcast(AbilityTags.First(), ItemData->CostTag, OutQuantity);
		}
		else if (!bWasSuccessful)
		{
			BaseASC->AbilityCostFailed.Broadcast();
		}
		return bWasSuccessful;
	}
	return false;
}

FString UBaseGameplayAbility::GetDescription(int32 Level)
{
	return FString::Printf(TEXT("<Default>%s, </><Level>%d</>"), L"Default Ability Name - LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum", Level);
}

FString UBaseGameplayAbility::GetNextLevelDescription(int32 Level)
{
	return FString::Printf(TEXT("<Default>Next Level: </><Level>%d</> \n<Default>Causes much more damage. </>"), Level);
}

FString UBaseGameplayAbility::GetLockedDescription(int32 Level)
{
	return FString::Printf(TEXT("<Default>Spell Locked Until Level: %d</>"), Level);
}

float UBaseGameplayAbility::GetManaCost(float InLevel) const
{
	float ManaCost = 0.f;
	if (const UGameplayEffect* CostEffect = GetCostGameplayEffect())
	{
		for (FGameplayModifierInfo Mod : CostEffect->Modifiers)
		{
			if (Mod.Attribute == UBaseAttributeSet::GetManaAttribute())
			{
				Mod.ModifierMagnitude.GetStaticMagnitudeIfPossible(InLevel, ManaCost);
				break;
			}
		}
	}
	return ManaCost;
}

float UBaseGameplayAbility::GetCooldown(float InLevel) const
{
	float Cooldown = 0.f;
	if (const UGameplayEffect* CooldownEffect = GetCooldownGameplayEffect())
	{
		CooldownEffect->DurationMagnitude.GetStaticMagnitudeIfPossible(InLevel, Cooldown);
	}
	return Cooldown;
}
