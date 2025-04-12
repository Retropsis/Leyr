// @ Retropsis 2024-2025.

#include "Data/AbilitySet.h"
#include "AbilitySystemComponent.h"
#include "ActiveGameplayEffectHandle.h"
#include "GameplayAbilitySpecHandle.h"
#include "AbilitySystem/Ability/BaseGameplayAbility.h"

void FAbilitySet_GrantedHandles::AddAbilitySpecHandle(const FGameplayAbilitySpecHandle& Handle)
{
	if (Handle.IsValid())
	{
		AbilitySpecHandles.Add(Handle);
	}
}

void FAbilitySet_GrantedHandles::AddGameplayEffectHandle(const FActiveGameplayEffectHandle& Handle)
{
	if (Handle.IsValid())
	{
		GameplayEffectHandles.Add(Handle);
	}
}

void FAbilitySet_GrantedHandles::TakeFromAbilitySystem(UAbilitySystemComponent* ASC)
{
	check(ASC);

	if (!ASC->IsOwnerActorAuthoritative())
	{
		// Must be authoritative to give or take ability sets.
		return;
	}

	for (const FGameplayAbilitySpecHandle& Handle : AbilitySpecHandles)
	{
		if (Handle.IsValid())
		{
			ASC->ClearAbility(Handle);
		}
	}

	for (const FActiveGameplayEffectHandle& Handle : GameplayEffectHandles)
	{
		if (Handle.IsValid())
		{
			ASC->RemoveActiveGameplayEffect(Handle);
		}
	}
	AbilitySpecHandles.Reset();
	GameplayEffectHandles.Reset();
}

void FAbilitySet_GrantedHandles::UpdateInputTags(const UAbilitySystemComponent* ASC, const FGameplayTagContainer& InputTags, ETagOperation TagOperation)
{
	check(ASC);

	if (!ASC->IsOwnerActorAuthoritative())
	{
		// Must be authoritative to give or take ability sets.
		return;
	}

	for (const FGameplayAbilitySpecHandle& Handle : AbilitySpecHandles)
	{
		if (Handle.IsValid())
		{
			FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromHandle(Handle);
			// if(Spec->AbilityTag.ToString().Contains("Execute"))
			// {
			// 	
			// }
			switch (TagOperation) {
			case ETagOperation::Reset:
				Spec->DynamicAbilityTags.Reset();
				break;
			case ETagOperation::Add:
				Spec->DynamicAbilityTags.AppendTags(InputTags);
				break;
			case ETagOperation::Remove:
				Spec->DynamicAbilityTags.RemoveTags(InputTags);
				break;
			case ETagOperation::Replace:
				Spec->DynamicAbilityTags.Reset();
				Spec->DynamicAbilityTags.AppendTags(InputTags);
				break;
			}
		}
	}
}

void UAbilitySet::GiveToAbilitySystem(UAbilitySystemComponent* ASC, FAbilitySet_GrantedHandles* OutGrantedHandles, FGameplayTag InputTag, float Level, UObject* SourceObject) const
{
	check(ASC);

	if (!ASC->IsOwnerActorAuthoritative())
	{
		// Must be authoritative to give or take ability sets.
		return;
	}

	// Grant the gameplay abilities.
	for (int32 AbilityIndex = 0; AbilityIndex < GrantedGameplayAbilities.Num(); ++AbilityIndex)
	{
		const FAbilitySet_GameplayAbility& AbilityToGrant = GrantedGameplayAbilities[AbilityIndex];

		if (!IsValid(AbilityToGrant.Ability))
		{
			UE_LOG(LogTemp, Error, TEXT("GrantedGameplayAbilities[%d] on ability set [%s] is not valid."), AbilityIndex, *GetNameSafe(this));
			continue;
		}

		UBaseGameplayAbility* AbilityCDO = AbilityToGrant.Ability->GetDefaultObject<UBaseGameplayAbility>();
		FGameplayAbilitySpec AbilitySpec(AbilityCDO, AbilityToGrant.Level);
		AbilitySpec.SourceObject = SourceObject;
		AbilitySpec.DynamicAbilityTags.AddTag(AbilityToGrant.InputTag.IsValid() ? AbilityToGrant.InputTag : InputTag);

		const FGameplayAbilitySpecHandle AbilitySpecHandle = ASC->GiveAbility(AbilitySpec);

		if (OutGrantedHandles)
		{
			OutGrantedHandles->AddAbilitySpecHandle(AbilitySpecHandle);
		}
	}

	// Grant the common abilities.
	for (int32 AbilityIndex = 0; AbilityIndex < CommonGameplayAbilities.Num(); ++AbilityIndex)
	{
		TSubclassOf<UGameplayAbility> AbilityToGrant = CommonGameplayAbilities[AbilityIndex];

		if (!IsValid(AbilityToGrant))
		{
			UE_LOG(LogTemp, Error, TEXT("CommonGameplayAbilities[%d] on ability set [%s] is not valid."), AbilityIndex, *GetNameSafe(this));
			continue;
		}

		UGameplayAbility* AbilityCDO = AbilityToGrant->GetDefaultObject<UGameplayAbility>();

		FGameplayAbilitySpec AbilitySpec(AbilityCDO, 1.f);
		AbilitySpec.SourceObject = SourceObject;

		const FGameplayAbilitySpecHandle AbilitySpecHandle = ASC->GiveAbility(AbilitySpec);

		if (OutGrantedHandles)
		{
			OutGrantedHandles->AddAbilitySpecHandle(AbilitySpecHandle);
		}
	}

	// Grant the gameplay effects.
	for (int32 EffectIndex = 0; EffectIndex < GrantedGameplayEffects.Num(); ++EffectIndex)
	{
		const FAbilitySet_GameplayEffect& EffectToGrant = GrantedGameplayEffects[EffectIndex];

		if (!IsValid(EffectToGrant.GameplayEffect))
		{
			UE_LOG(LogTemp, Error, TEXT("GrantedGameplayEffects[%d] on ability set [%s] is not valid"), EffectIndex, *GetNameSafe(this));
			continue;
		}

		const UGameplayEffect* GameplayEffect = EffectToGrant.GameplayEffect->GetDefaultObject<UGameplayEffect>();
		FGameplayEffectContextHandle ContextHandle =ASC->MakeEffectContext();
		ContextHandle.AddSourceObject(ASC->GetAvatarActor());
		const FActiveGameplayEffectHandle GameplayEffectHandle = ASC->ApplyGameplayEffectToSelf(GameplayEffect, Level, ContextHandle);

		if (OutGrantedHandles)
		{
			OutGrantedHandles->AddGameplayEffectHandle(GameplayEffectHandle);
		}
	}
}
