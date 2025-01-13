// @ Retropsis 2024-2025.

#include "AbilitySystem/Ability/MultiHitMeleeAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Game/BaseGameplayTags.h"

void UMultiHitMeleeAbility::SelectMontageTagFromComboSequenceIndex()
{
	switch (ComboSequenceIndex)
	{
	case 1: MontageTag = FBaseGameplayTags::Get().Montage_Attack_2; break;
	case 2: MontageTag = FBaseGameplayTags::Get().Montage_Attack_3; break;
	case 3: MontageTag = FBaseGameplayTags::Get().Montage_Attack_4; break;
	default: MontageTag = FBaseGameplayTags::Get().Montage_Attack_1; break;
	}
}

void UMultiHitMeleeAbility::SelectMontageTagCombo()
{
	const FBaseGameplayTags GameplayTags = FBaseGameplayTags::Get();
	const FGameplayTagContainer OwnedTags = GetAbilitySystemComponentFromActorInfo()->GetOwnedGameplayTags();
	
	if (OwnedTags.HasTagExact(GameplayTags.Abilities_Combo_Window ))
	{
		UAbilitySystemBlueprintLibrary::RemoveLooseGameplayTags(GetAvatarActorFromActorInfo(), FGameplayTagContainer{ FBaseGameplayTags::Get().Abilities_Combo_Window });
		
		if (OwnedTags.HasTagExact(GameplayTags.Abilities_Combo_Attack_1 ))
		{
			MontageTag = GameplayTags.Montage_Attack_2;
			UAbilitySystemBlueprintLibrary::RemoveLooseGameplayTags(GetAvatarActorFromActorInfo(), FGameplayTagContainer{ FBaseGameplayTags::Get().Abilities_Combo_Attack_1 });
			UAbilitySystemBlueprintLibrary::AddLooseGameplayTags(GetAvatarActorFromActorInfo(), FGameplayTagContainer{ FBaseGameplayTags::Get().Abilities_Combo_Attack_2 });
			return;
		}
		if (OwnedTags.HasTagExact(GameplayTags.Abilities_Combo_Attack_2 ))
		{
			MontageTag = GameplayTags.Montage_Attack_3;
			UAbilitySystemBlueprintLibrary::RemoveLooseGameplayTags(GetAvatarActorFromActorInfo(), FGameplayTagContainer{ FBaseGameplayTags::Get().Abilities_Combo_Attack_2 });
			UAbilitySystemBlueprintLibrary::AddLooseGameplayTags(GetAvatarActorFromActorInfo(), FGameplayTagContainer{ FBaseGameplayTags::Get().Abilities_Combo_Attack_3 });
			return;
		}
		if (OwnedTags.HasTagExact(GameplayTags.Abilities_Combo_Attack_3 ))
		{
			MontageTag = GameplayTags.Montage_Attack_4;
			UAbilitySystemBlueprintLibrary::RemoveLooseGameplayTags(GetAvatarActorFromActorInfo(), FGameplayTagContainer{ FBaseGameplayTags::Get().Abilities_Combo_Attack_3 });
			bShouldEndAbility = true;
		}
	}
	else
	{
		MontageTag = GameplayTags.Montage_Attack_1;
		UAbilitySystemBlueprintLibrary::AddLooseGameplayTags(GetAvatarActorFromActorInfo(), FGameplayTagContainer{ FBaseGameplayTags::Get().Abilities_Combo_Attack_1 });
		UAbilitySystemBlueprintLibrary::RemoveLooseGameplayTags(GetAvatarActorFromActorInfo(), FGameplayTagContainer{ FBaseGameplayTags::Get().Abilities_Combo_Attack_2 });
		UAbilitySystemBlueprintLibrary::RemoveLooseGameplayTags(GetAvatarActorFromActorInfo(), FGameplayTagContainer{ FBaseGameplayTags::Get().Abilities_Combo_Attack_3 });
	}
	
}

void UMultiHitMeleeAbility::OnComboWindowBegin()
{
	UAbilitySystemBlueprintLibrary::AddLooseGameplayTags(GetAvatarActorFromActorInfo(), FGameplayTagContainer{ FBaseGameplayTags::Get().Abilities_Combo_Window });
}

void UMultiHitMeleeAbility::OnComboWindowEnd()
{
	UAbilitySystemBlueprintLibrary::RemoveLooseGameplayTags(GetAvatarActorFromActorInfo(), FGameplayTagContainer{ FBaseGameplayTags::Get().Abilities_Combo_Window });
	UAbilitySystemBlueprintLibrary::RemoveLooseGameplayTags(GetAvatarActorFromActorInfo(), FGameplayTagContainer{ FBaseGameplayTags::Get().Abilities_Combo_Attack_1 });
	UAbilitySystemBlueprintLibrary::RemoveLooseGameplayTags(GetAvatarActorFromActorInfo(), FGameplayTagContainer{ FBaseGameplayTags::Get().Abilities_Combo_Attack_2 });
	UAbilitySystemBlueprintLibrary::RemoveLooseGameplayTags(GetAvatarActorFromActorInfo(), FGameplayTagContainer{ FBaseGameplayTags::Get().Abilities_Combo_Attack_3 });
}
