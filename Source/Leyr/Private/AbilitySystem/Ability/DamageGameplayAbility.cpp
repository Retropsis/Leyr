// @ Retropsis 2024-2025.

#include "AbilitySystem/Ability/DamageGameplayAbility.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/LeyrAbilitySystemLibrary.h"
#include "Game/BaseGameplayTags.h"
#include "GameplayEffectComponents/TargetTagsGameplayEffectComponent.h"
#include "Interaction/InteractionInterface.h"
#include "Kismet/KismetSystemLibrary.h"

void UDamageGameplayAbility::InitAbility()
{
	Super::InitAbility();
	bHasHitTarget = false;
	ICombatInterface::Execute_SetMovementEnabled(GetAvatarActorFromActorInfo(), false);
	PaperAnimInstance = ICombatInterface::Execute_GetPaperAnimInstance(GetAvatarActorFromActorInfo());
}

void UDamageGameplayAbility::PrepareToEndAbility()
{
	Super::PrepareToEndAbility();
	ICombatInterface::Execute_SetMovementEnabled(GetAvatarActorFromActorInfo(), true);
}

TArray<FHitResult> UDamageGameplayAbility::BoxTrace(bool bDebug)
{
	BoxTraceData = ICombatInterface::Execute_GetBoxTraceDataByTag(GetAvatarActorFromActorInfo(), MontageTag, SequenceType);
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	TEnumAsByte<EObjectTypeQuery> OT = GetAvatarActorFromActorInfo()->ActorHasTag(FName("Player")) ? EOT_EnemyCapsule : ObjectTypeQuery3;
	ObjectTypes.Add(OT);
	// ObjectTypes.Add(ICombatInterface::Execute_GetTraceObjectType(GetAvatarActorFromActorInfo()));
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(GetAvatarActorFromActorInfo());

	TArray<FHitResult> Hits;
	UKismetSystemLibrary::BoxTraceMultiForObjects(
		this, BoxTraceData.Start, BoxTraceData.End, BoxTraceData.Extent, FRotator::ZeroRotator, ObjectTypes,
		false, ActorsToIgnore, bDebug ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None, Hits, true);
	
	for (FHitResult HitResult : Hits)
	{
		if(HitResult.bBlockingHit && HitResult.GetActor() && HitResult.GetActor()->ActorHasTag("HitInteraction"))
		{
			IInteractionInterface::Execute_Interact(HitResult.GetActor(), GetAvatarActorFromActorInfo());
		}
	}
	return Hits;
}

void UDamageGameplayAbility::CauseDamage(UAbilitySystemComponent* TargetASC)
{
	FGameplayEffectSpecHandle DamageSpecHandle = MakeOutgoingGameplayEffectSpec(DamageEffectClass, GetAbilityLevel());
	const float ScaledDamage =AbilityPower.GetRandomFloatFromScalableRange(GetAbilityLevel());
	
	if (TargetASC->HasMatchingGameplayTag(FBaseGameplayTags::Get().Indicator_Execute))
	{
		DamageType = FBaseGameplayTags::Get().Damage_Execute;
	}
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle, DamageType, ScaledDamage);
	if(bShouldApplyExecute) MakeAndApplyExecuteEffectToTarget(FBaseGameplayTags::Get().Indicator_Execute, TargetASC);
	
	GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(*DamageSpecHandle.Data.Get(), TargetASC);
}

void UDamageGameplayAbility::ForEachHitTryCausingDamage(TArray<FHitResult> HitResults)
{
	for (FHitResult Hit : HitResults)
	{
		if(Hit.bBlockingHit && Hit.GetActor() && ULeyrAbilitySystemLibrary::IsHostile(GetAvatarActorFromActorInfo(), Hit.GetActor()))
		{
			HitActor = Hit.GetActor();
			if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(HitActor))
			{
				CauseDamage(TargetASC);
			}
			bHasHitTarget = true;
		}
	}
}

void UDamageGameplayAbility::MakeAndApplyExecuteEffectToTarget(const FGameplayTag& TagToApply, UAbilitySystemComponent* TargetASC, int32 Level)
{
	const FBaseGameplayTags& GameplayTags = FBaseGameplayTags::Get();
	FGameplayEffectContextHandle EffectContext = GetAbilitySystemComponentFromActorInfo()->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	FString TagName = FString::Printf(TEXT("%s"), *TagToApply.ToString());
	UGameplayEffect* Effect = NewObject<UGameplayEffect>(GetTransientPackage(), FName(TagName));

	Effect->DurationPolicy = EGameplayEffectDurationType::HasDuration;
	Effect->DurationMagnitude = FGameplayEffectModifierMagnitude{ 1.25f };
	
	UTargetTagsGameplayEffectComponent& AssetTagsComponent = Effect->FindOrAddComponent<UTargetTagsGameplayEffectComponent>();
	FInheritedTagContainer InheritedTagContainer;
	InheritedTagContainer.Added.AddTag(TagToApply);
	AssetTagsComponent.SetAndApplyTargetTagChanges(InheritedTagContainer);
	
	Effect->StackingType = EGameplayEffectStackingType::AggregateByTarget;
	Effect->StackLimitCount = 1;

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

		GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(*MutableSpec, TargetASC);
	}
}

bool UDamageGameplayAbility::IsHostile() const
{
	return ULeyrAbilitySystemLibrary::IsHostile(GetAvatarActorFromActorInfo(), HitActor);
}

void UDamageGameplayAbility::SetCurrentComboSequence()
{
	const int32 Index = ICombatInterface::Execute_GetAttackComboIndex(GetAvatarActorFromActorInfo());
	FTaggedMontage TaggedMontage = ICombatInterface::Execute_GetTaggedMontageByIndex(GetAvatarActorFromActorInfo(), Index);
	SelectedMontage = TaggedMontage.Montage;
	MontageTag = TaggedMontage.MontageTag;
}

void UDamageGameplayAbility::SetCurrentSequence()
{
	FTaggedMontage TaggedMontage = ICombatInterface::Execute_GetTaggedMontageByTag(GetAvatarActorFromActorInfo(), MontageTag, SequenceType);
	SelectedMontage = TaggedMontage.Montage;
}

void UDamageGameplayAbility::SelectMontageTagFromCombatState()
{
	const FBaseGameplayTags GameplayTags = FBaseGameplayTags::Get();
	FGameplayTagContainer OwnedTags = GetAbilitySystemComponentFromActorInfo()->GetOwnedGameplayTags();
	MontageTag = GameplayTags.Montage_Attack_1;
	
	if (OwnedTags.HasAllExact(FGameplayTagContainer{ GameplayTags.CombatState_Falling }))
	{
		MontageTag = GameplayTags.Montage_Jump_Attack;
	}
	TArray<FGameplayTag>JumpDownAttackTags;
	JumpDownAttackTags.Add(GameplayTags.CombatState_Unoccupied);
	JumpDownAttackTags.Add(GameplayTags.CombatState_Directional_Upward);
	if (OwnedTags.HasAllExact(FGameplayTagContainer::CreateFromArray(JumpDownAttackTags)))
	{
		MontageTag = GameplayTags.Montage_JumpDown_Attack;
	}
	if (OwnedTags.HasAllExact(FGameplayTagContainer{ GameplayTags.CombatState_Crouching }))
	{
		MontageTag = GameplayTags.Montage_Crouch_Attack;
	}
	TArray<FGameplayTag>UpwardAttackTags;
	UpwardAttackTags.Add(GameplayTags.CombatState_Unoccupied);
	UpwardAttackTags.Add(GameplayTags.CombatState_Directional_Upward);
	if (OwnedTags.HasAllExact(FGameplayTagContainer::CreateFromArray(UpwardAttackTags)))
	{
		MontageTag = GameplayTags.Montage_Upward_Attack;
	}
	if (OwnedTags.HasAllExact(FGameplayTagContainer{ GameplayTags.CombatState_Rolling }))
	{
		MontageTag = GameplayTags.Montage_Roll_Attack;
	}
	if (OwnedTags.HasAllExact(FGameplayTagContainer{ GameplayTags.CombatState_Rope }))
	{
		MontageTag = GameplayTags.Montage_Rope_Attack;
	}
	if (OwnedTags.HasAllExact(FGameplayTagContainer{ GameplayTags.CombatState_Ladder }))
	{
		MontageTag = GameplayTags.Montage_Ladder_Attack;
	}
	if (OwnedTags.HasAllExact(FGameplayTagContainer{ GameplayTags.CombatState_Slope }))
	{
		MontageTag = GameplayTags.Montage_Slope_Attack;
	}
	if (OwnedTags.HasAllExact(FGameplayTagContainer{ GameplayTags.CombatState_Swimming }))
	{
		MontageTag = GameplayTags.Montage_Swim_Attack;
	}
}

FAdditionalEffectParams UDamageGameplayAbility::MakeAdditionalEffectParamsFromClassDefaults(AActor* TargetActor) const
{
	FAdditionalEffectParams Params;
	Params.WorldContextObject = GetAvatarActorFromActorInfo();

	// TODO: Could Additional Effect Class be specialised ? (Using AdditionalEffectClass instead of DamageEffectClass)
	Params.AdditionalEffectClass = DamageEffectClass;
	Params.SourceAbilitySystemComponent = GetAbilitySystemComponentFromActorInfo();
	Params.TargetAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	Params.AbilityPower = AbilityPower.GetRandomFloatFromScalableRange(GetAbilityLevel());
	Params.AbilityLevel = GetAbilityLevel();
	Params.DamageType = DamageType;
	Params.StatusEffectChance = StatusEffectChance;
	Params.StatusEffectDamage = StatusEffectDamage;
	Params.StatusEffectDuration = StatusEffectDuration;
	Params.StatusEffectFrequency = StatusEffectFrequency;
	Params.DeathImpulseMagnitude = DeathImpulseMagnitude;
	Params.AirborneForceMagnitude = AirborneForceMagnitude;
	Params.AirborneChance = AirborneChance;
	if (IsValid(TargetActor))
	{
		FRotator Rotation = (TargetActor->GetActorLocation() - GetAvatarActorFromActorInfo()->GetActorLocation()).Rotation();
		Rotation.Pitch = 45.f;
		const FVector ToTarget = Rotation.Vector();
		Params.DeathImpulse = ToTarget * DeathImpulseMagnitude;
		if(FMath::RandRange(1, 100) < Params.AirborneChance) Params.AirborneForce = ToTarget * AirborneForceMagnitude;
	}
	return Params;
}

FTaggedMontage UDamageGameplayAbility::GetRandomTaggedMontageFromArray(const TArray<FTaggedMontage>& TaggedMontages) const
{
	if (TaggedMontages.Num() > 0)
	{
		const int32 Selection = FMath::RandRange(0, TaggedMontages.Num() - 1);
		return TaggedMontages[Selection];
	}
	return FTaggedMontage();
}

// FVector2D UDamageGameplayAbility::GetDamageByDamageType(float InLevel, const FGameplayTag& DamageType)
// {
// 	checkf(DamageTypes.Contains(DamageType), TEXT("GameplayAbilit [%s] does not contain DamageType [%s]"), *GetNameSafe(this), *DamageType.ToString());
// 	return FVector2D(DamageTypes[DamageType].ValueMin.GetValueAtLevel(InLevel), DamageTypes[DamageType].ValueMax.GetValueAtLevel(InLevel));
// }
