// @ Retropsis 2024-2025.

#include "AbilitySystem/Ability/BaseGameplayAbility.h"
#include "PaperZDAnimInstance.h"
#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "AbilitySystem/BaseAttributeSet.h"
#include "AbilitySystem/LeyrAbilitySystemLibrary.h"
#include "AbilitySystem/Cue/GameplayCueDefinition.h"
#include "AI/BaseCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Data/AbilityData.h"
#include "Data/ItemData.h"
#include "Game/BaseGameplayTags.h"
#include "GameplayEffectComponents/TargetTagsGameplayEffectComponent.h"
#include "Interaction/CombatInterface.h"
#include "Interaction/PlayerInterface.h"

void UBaseGameplayAbility::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);
	TryActivateAbilityOnSpawn(ActorInfo, Spec);
}

void UBaseGameplayAbility::TryActivateAbilityOnSpawn(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) const
{
	const bool bIsPredicting = (Spec.ActivationInfo.ActivationMode == EGameplayAbilityActivationMode::Predicting);

	// Try to activate if activation policy is on spawn.
	if (ActorInfo && !Spec.IsActive() && !bIsPredicting && (ActivationPolicy == EAbilityActivationPolicy::OnSpawn))
	{
		UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();
		const AActor* AvatarActor = ActorInfo->AvatarActor.Get();

		// If avatar actor is torn off or about to die, don't try to activate until we get the new one.
		if (ASC && AvatarActor && !AvatarActor->GetTearOff() && (AvatarActor->GetLifeSpan() <= 0.0f))
		{
			const bool bIsLocalExecution = (NetExecutionPolicy == EGameplayAbilityNetExecutionPolicy::LocalPredicted) || (NetExecutionPolicy == EGameplayAbilityNetExecutionPolicy::LocalOnly);
			const bool bIsServerExecution = (NetExecutionPolicy == EGameplayAbilityNetExecutionPolicy::ServerOnly) || (NetExecutionPolicy == EGameplayAbilityNetExecutionPolicy::ServerInitiated);

			const bool bClientShouldActivate = ActorInfo->IsLocallyControlled() && bIsLocalExecution;
			const bool bServerShouldActivate = ActorInfo->IsNetAuthority() && bIsServerExecution;

			if (bClientShouldActivate || bServerShouldActivate)
			{
				ASC->TryActivateAbility(Spec.Handle);
			}
		}
	}
}

void UBaseGameplayAbility::InitAbility()
{
	if (FetchAnimInstances()) return;
	
	GameplayCueDefinition = NewObject<UGameplayCueDefinition>();
	DamageType = FBaseGameplayTags::Get().Damage_Physical;
	
	InitializeDefaultAbilityData();
	InitializeItemAbilityData();
	DeterminePoise();
}

bool UBaseGameplayAbility::FetchAnimInstances()
{
	if (!GetAvatarActorFromActorInfo()->Implements<UCombatInterface>()) return true;
	
	if (bResetPitch) ICombatInterface::Execute_ResetAimingPitch(GetAvatarActorFromActorInfo());
	PaperAnimInstance = ICombatInterface::Execute_GetPaperAnimInstance(GetAvatarActorFromActorInfo());
	UpperBodyAnimInstance = ICombatInterface::Execute_GetUpperBodyAnimInstance(GetAvatarActorFromActorInfo());
	WeaponAnimInstance = ICombatInterface::Execute_GetWeaponAnimInstance(GetAvatarActorFromActorInfo());
	if (WeaponAnimInstance) WeaponAnimInstance->StopAllAnimationOverrides();
	
	return false;
}

/*
 * Initialize from Default Ability Data asset
 */
void UBaseGameplayAbility::InitializeDefaultAbilityData()
{
	if (AbilityData)
	{
		AbilityPower = AbilityData->AbilityPower.GetValueAtLevel(GetAbilityLevel());
		AbilityPoise = AbilityData->AbilityPoise;
		SequenceType = AbilityData->SequenceType;
		DamageType = AbilityData->DamageType;
	} 
	else if (AbilityData = ICombatInterface::Execute_LoadAndGetDefaultAbilityData(GetAvatarActorFromActorInfo()); AbilityData)
	{
		SequenceType = AbilityData->SequenceType;
		DamageType = AbilityData->DamageType;
	}
}

/*
 * Initialize from Item Data asset
 */
void UBaseGameplayAbility::InitializeItemAbilityData()
{
	if (AbilityItemData = Cast<UItemData>(GetSourceObjectFromAbilitySpec()); AbilityItemData)
	{
		if (AbilityItemData->AnimationInstance)
		{
			if (const ABaseCharacter* Character = Cast<ABaseCharacter>(GetAvatarActorFromActorInfo()))
			{
				WeaponAnimInstance = Character->SetWeaponAnimInstance(AbilityItemData->AnimationInstance);
			}
		}
		if (AbilityItemData->AbilityDataOverride)
		{
			AbilityData = AbilityItemData->AbilityDataOverride;
			
			AbilityPower = AbilityData->AbilityPower.GetValueAtLevel(GetAbilityLevel());
			AbilityPoise = AbilityData->AbilityPoise;
			SequenceType = AbilityData->SequenceType;
			DamageType = AbilityData->DamageType;
		}
	}
}

void UBaseGameplayAbility::DeterminePoise()
{	
	const FBaseGameplayTags& GameplayTags = FBaseGameplayTags::Get();
	FGameplayEffectContextHandle EffectContext = GetAbilitySystemComponentFromActorInfo()->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	FString EffectName = FString::Printf(TEXT("Ability_Poise"));
	UGameplayEffect* Effect = NewObject<UGameplayEffect>(this, FName(EffectName));
	Effect->DurationPolicy = EGameplayEffectDurationType::Infinite;
	
	// UTargetTagsGameplayEffectComponent& AssetTagsComponent = Effect->FindOrAddComponent<UTargetTagsGameplayEffectComponent>();
	// FInheritedTagContainer InheritedTagContainer;
	// InheritedTagContainer.Added.AddTag(TagToApply);
	// AssetTagsComponent.SetAndApplyTargetTagChanges(InheritedTagContainer);

	FGameplayModifierInfo ModifierInfo;
	ModifierInfo.Attribute = UBaseAttributeSet::GetPoiseAttribute();
	ModifierInfo.ModifierOp = EGameplayModOp::Additive;
	ModifierInfo.ModifierMagnitude = FScalableFloat(AbilityPoise);
	Effect->Modifiers.Add(ModifierInfo);
	
	ActivePoiseEffectHandle = GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectToTarget(Effect, GetAbilitySystemComponentFromActorInfo(), GetAbilityLevel(), EffectContext);
}

void UBaseGameplayAbility::RemovePoise() const
{
	if (ActivePoiseEffectHandle.IsValid()) GetAbilitySystemComponentFromActorInfo()->RemoveActiveGameplayEffect(ActivePoiseEffectHandle);
}

void UBaseGameplayAbility::InitAbilityWithParams(FInitAbilityParams Params)
{
	bResetPitch = Params.bResetPitch;
	GetAbilitySystemComponentFromActorInfo()->AddLooseGameplayTags(Params.TagsToApply);
	InitAbility();
}

void UBaseGameplayAbility::PrepareToEndAbility()
{
	RemovePoise();
}

void UBaseGameplayAbility::PrepareToEndAbilityWithParams(FEndAbilityParams Params)
{
	bResetPitch = Params.bResetPitch;
	GetAbilitySystemComponentFromActorInfo()->RemoveLooseGameplayTags(Params.TagsToRemove);
	PrepareToEndAbility();
}

void UBaseGameplayAbility::SetCurrentSequence()
{
	TaggedMontage = ICombatInterface::Execute_GetTaggedMontageByTag(GetAvatarActorFromActorInfo(), MontageTag, SequenceType);
	SelectedMontage = TaggedMontage.Montage;
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
	Effect->StackLimitCount = 3;
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

void UBaseGameplayAbility::SetOwningCharacterCapsuleCollision(const ECollisionChannel Channel, const ECollisionResponse Response) const
{
	if (const ACharacter* Character = Cast<ACharacter>(GetAvatarActorFromActorInfo()))
	{
		Character->GetCapsuleComponent()->SetCollisionResponseToChannel(Channel, Response);
	}
}

void UBaseGameplayAbility::SetGCDImpactSound(UObject* Object) const
{
	if (GameplayCueDefinition) GameplayCueDefinition->ImpactSound = Cast<USoundBase>(Object);
}

TSoftObjectPtr<USoundBase> UBaseGameplayAbility::GetImpactSoundAsset() const
{
	if (GetAvatarActorFromActorInfo() && GetAvatarActorFromActorInfo()->Implements<UCombatInterface>())
	{
		return ICombatInterface::Execute_ImpactSoundFromTag(GetAvatarActorFromActorInfo(), MontageTag, SequenceType);
	}
	return nullptr;
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

void UBaseGameplayAbility::SetAbilityData(UAbilityData* Data)
{
	if (IsValid(Data))
	{
		AbilityData = Data;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Attempted to set a nullptr Data in [%s]"), *GetNameSafe(this))
	}
}