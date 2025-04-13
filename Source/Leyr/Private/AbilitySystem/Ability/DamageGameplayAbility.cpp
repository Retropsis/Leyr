// @ Retropsis 2024-2025.

#include "AbilitySystem/Ability/DamageGameplayAbility.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/LeyrAbilitySystemLibrary.h"
#include "AbilitySystem/Cue/GameplayCueDefinition.h"
#include "AI/BaseCharacter.h"
#include "Data/AbilityData.h"
#include "Data/ItemData.h"
#include "Data/StatusEffectData.h"
#include "Game/BaseGameplayTags.h"
#include "Interaction/InteractionInterface.h"
#include "Kismet/KismetSystemLibrary.h"

void UDamageGameplayAbility::InitAbility()
{
	Super::InitAbility();
	bHasHitTarget = false;
	ICombatInterface::Execute_SetMovementEnabled(GetAvatarActorFromActorInfo(), false);
	
	if (AbilityData)
	{
		DamageEffectClass = AbilityData->DamageEffectClass;
		bIsRadialDamage = AbilityData->bIsRadialDamage;
		RadialDamageInnerRadius = AbilityData->RadialDamageInnerRadius;
		RadialDamageOuterRadius = AbilityData->RadialDamageOuterRadius;
		DeathImpulseMagnitude = AbilityData->DeathImpulseMagnitude;
		AirborneChance = AbilityData->AirborneChance;
		AirborneForceMagnitude = AbilityData->AirborneForceMagnitude;
		
		// bShouldApplyExecute = AbilityData->bShouldApplyExecute;
		// bShouldExecute = AbilityData->bShouldExecute;
	}
}

void UDamageGameplayAbility::PrepareToEndAbility()
{
	Super::PrepareToEndAbility();
	ICombatInterface::Execute_SetMovementEnabled(GetAvatarActorFromActorInfo(), true);
	HitActor = nullptr;
	HitLocation = FVector::ZeroVector;
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
		if(HitResult.bBlockingHit && HitResult.GetActor() && HitResult.GetActor()->Implements<UInteractionInterface>())
		{
			IInteractionInterface::Execute_InteractHit(HitResult.GetActor(), GetAvatarActorFromActorInfo());
		}
	}
	return Hits;
}

void UDamageGameplayAbility::CauseDamage(UAbilitySystemComponent* TargetASC)
{
	const FBaseGameplayTags& GameplayTags = FBaseGameplayTags::Get();
	const FGameplayEffectSpecHandle DamageSpecHandle = MakeOutgoingGameplayEffectSpec(DamageEffectClass, GetAbilityLevel());
	const float ScaledDamage =AbilityPower.GetValueAtLevel(GetAbilityLevel());
	
	if (bShouldExecute)
	{
		if (TargetASC->HasMatchingGameplayTag(GameplayTags.Execute))
		{
			DamageType = GameplayTags.Damage_Execute;
			bExecuteSuccessful = true;
		}
		else
		{
			// Weaker hit
			return;
		}
	}
	
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle, DamageType, ScaledDamage);
	if(bShouldApplyExecute) MakeAndApplyExecuteEffectToTarget(GameplayTags.Indicator_Execute, TargetASC);
	
	GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(*DamageSpecHandle.Data.Get(), TargetASC);

	// Status Effect Application
	if (AbilityData && AbilityData->StatusEffectData) AbilityData->StatusEffectData->ApplyStatusEffect(GetAbilitySystemComponentFromActorInfo(), TargetASC); 
}

void UDamageGameplayAbility::ForEachHitTryCausingDamage(TArray<FHitResult> HitResults)
{
	HitActors.Empty();
	
	for (FHitResult Hit : HitResults)
	{
		if(Hit.bBlockingHit && Hit.GetActor() && ULeyrAbilitySystemLibrary::IsHostile(GetAvatarActorFromActorInfo(), Hit.GetActor()))
		{
			HitActor = Hit.GetActor();
			if(HitActors.Contains(HitActor)) continue;
			
			if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(HitActor))
			{
				CauseDamage(TargetASC);
				HitLocation = (Hit.GetActor()->GetActorLocation() + Hit.ImpactPoint) / 2.f;
				HitLocation.Y = 0.f;
				HitActors.AddUnique(HitActor);
			}
			bHasHitTarget = true;
		}
	}
}

float UDamageGameplayAbility::GetAbilityPowerAtLevel() const
{
	return AbilityPower.GetValueAtLevel(GetAbilityLevel());
}

void UDamageGameplayAbility::ApplyHitStop()
{
	const bool bShouldHitStop = WeaponTaggedMontage.HitStopDuration > 0.f && bHasHitTarget;
	if (HitActors.Num() <= 0 || !bShouldHitStop) return;

	for (AActor* Actor : HitActors)
	{
		if(ABaseCharacter* Character = Cast<ABaseCharacter>(Actor)) Character->HitStop(WeaponTaggedMontage.HitStopDuration, WeaponTaggedMontage.HitStopAmount);
	}
	if(ABaseCharacter* Character = Cast<ABaseCharacter>(GetAvatarActorFromActorInfo()))
	{
		Character->HitStop(WeaponTaggedMontage.HitStopDuration, WeaponTaggedMontage.HitStopAmount);
	}
}

bool UDamageGameplayAbility::IsHostile() const
{
	return ULeyrAbilitySystemLibrary::IsHostile(GetAvatarActorFromActorInfo(), HitActor);
}

void UDamageGameplayAbility::SetCurrentComboSequence()
{
	const int32 Index = ICombatInterface::Execute_GetAttackComboIndex(GetAvatarActorFromActorInfo());
	TaggedMontage = ICombatInterface::Execute_GetTaggedMontageByIndex(GetAvatarActorFromActorInfo(), Index);
	SelectedMontage = TaggedMontage.Montage;
	MontageTag = TaggedMontage.MontageTag;
}

void UDamageGameplayAbility::SetCurrentSequence()
{
	TaggedMontage = ICombatInterface::Execute_GetTaggedMontageByTag(GetAvatarActorFromActorInfo(), MontageTag, SequenceType);
	SelectedMontage = TaggedMontage.Montage;

	if (AbilityItemData)
	{
		WeaponTaggedMontage = AbilityItemData->FindSequenceInfoForTag(MontageTag);
		SelectedWeaponMontage = WeaponTaggedMontage.Montage;
		HitImpulse = WeaponTaggedMontage.HitImpulse;
	}
}

void UDamageGameplayAbility::SelectMontageTagFromCombatState()
{
	const FBaseGameplayTags GameplayTags = FBaseGameplayTags::Get();
	const FGameplayTagContainer OwnedTags = GetAbilitySystemComponentFromActorInfo()->GetOwnedGameplayTags();
	MontageTag = GameplayTags.Montage_Attack_1;
	
	if (OwnedTags.HasTagExact(GameplayTags.CombatState_Condition_Falling ))
	{
		if (OwnedTags.HasTagExact(GameplayTags.CombatState_Directional_Downward )) { MontageTag = GameplayTags.Montage_Falling_Attack; return; }
		if (OwnedTags.HasTagExact(GameplayTags.CombatState_Directional_ForwardUp )) { MontageTag = GameplayTags.Montage_JumpForwardUp_Attack; return; }
		if (OwnedTags.HasTagExact(GameplayTags.CombatState_Directional_ForwardDown )) { MontageTag = GameplayTags.Montage_JumpForwardDown_Attack; return; }
		if (OwnedTags.HasTagExact(GameplayTags.CombatState_Directional_Upward )) { MontageTag = GameplayTags.Montage_JumpUpward_Attack; return; }
		MontageTag = GameplayTags.Montage_Jump_Attack; return;
	}
	if (OwnedTags.HasTagExact(GameplayTags.CombatState_Condition_Crouching )) { MontageTag = GameplayTags.Montage_Crouch_Attack; return; }
	if (OwnedTags.HasTagExact(GameplayTags.CombatState_Directional_Upward )) { MontageTag = GameplayTags.Montage_Upward_Attack; return; }
	if (OwnedTags.HasTagExact(GameplayTags.CombatState_Directional_ForwardUp )) { MontageTag = GameplayTags.Montage_ForwardUp_Attack; return; }
	if (OwnedTags.HasTagExact(GameplayTags.CombatState_Transient_Rolling )) { MontageTag = GameplayTags.Montage_Roll_Attack; return; }
	if (OwnedTags.HasTagExact(GameplayTags.CombatState_Condition_Rope )) { MontageTag = GameplayTags.Montage_Rope_Attack; return; }
	if (OwnedTags.HasTagExact(GameplayTags.CombatState_Condition_Ladder )) { MontageTag = GameplayTags.Montage_Ladder_Attack; return; }
	if (OwnedTags.HasTagExact(GameplayTags.CombatState_Condition_Slope )) { MontageTag = GameplayTags.Montage_Slope_Attack; return; }
	if (OwnedTags.HasTagExact(GameplayTags.CombatState_Condition_Swimming )) { MontageTag = GameplayTags.Montage_Swim_Attack; }
}

void UDamageGameplayAbility::ExecuteDamageGameplayCue(FGameplayTag GameplayCueTag)
{
	if (HitActor->Implements<UCombatInterface>()) GameplayCueDefinition->ImpactEffect = ICombatInterface::Execute_GetImpactEffect(HitActor);
		
	FGameplayCueParameters GameplayCueParameters;
	GameplayCueParameters.Location = HitLocation;
	GameplayCueParameters.EffectCauser = GetAvatarActorFromActorInfo();
	GameplayCueParameters.SourceObject = GameplayCueDefinition;
	GameplayCueParameters.AggregatedSourceTags = MontageTag.GetSingleTagContainer();

	if (UAbilitySystemComponent* const AbilitySystemComponent = GetAbilitySystemComponentFromActorInfo())
	{
		AbilitySystemComponent->ExecuteGameplayCue(GameplayCueTag, GameplayCueParameters);
	}
}

FAdditionalEffectParams UDamageGameplayAbility::MakeAdditionalEffectParamsFromClassDefaults(AActor* TargetActor) const
{
	FAdditionalEffectParams Params;
	Params.WorldContextObject = GetAvatarActorFromActorInfo();
	Params.AdditionalEffectClass = DamageEffectClass;
	Params.SourceObject = GetSourceObject(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo());
	Params.SourceAbilitySystemComponent = GetAbilitySystemComponentFromActorInfo();
	Params.TargetAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	Params.AbilityPower = AbilityPower.GetValueAtLevel(GetAbilityLevel());
	Params.AbilityLevel = GetAbilityLevel();
	Params.DamageType = DamageType;
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
	if (bIsRadialDamage)
	{
		Params.bIsRadialDamage = bIsRadialDamage;
		Params.RadialDamageInnerRadius = RadialDamageInnerRadius;
		Params.RadialDamageOuterRadius = RadialDamageOuterRadius;
	}
	return Params;
}

// FStatusEffectParams UDamageGameplayAbility::MakeStatusEffectParamsFromClassDefaults(AActor* TargetActor) const
// {
// 	FStatusEffectParams Params;
// 	Params.WorldContextObject = GetAvatarActorFromActorInfo();
// 	Params.AdditionalEffectClass = StatusEffectClass;
// 	Params.SourceObject = GetSourceObject(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo());
// 	Params.SourceAbilitySystemComponent = GetAbilitySystemComponentFromActorInfo();
// 	Params.TargetAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
// 	Params.AbilityPower = AbilityPower.GetValueAtLevel(GetAbilityLevel());
// 	Params.AbilityLevel = GetAbilityLevel();
// 	Params.DamageType = DamageType;
// 	Params.StatusEffectType = StatusEffectType;
// 	Params.StatusEffectChance = StatusEffectChance;
// 	Params.StatusEffectDamage = StatusEffectDamage;
// 	Params.StatusEffectDuration = StatusEffectDuration;
// 	Params.StatusEffectFrequency = StatusEffectFrequency;
// 	return Params;
// }

FTaggedMontage UDamageGameplayAbility::GetRandomTaggedMontageFromArray(const TArray<FTaggedMontage>& TaggedMontages) const
{
	if (TaggedMontages.Num() > 0)
	{
		const int32 Selection = FMath::RandRange(0, TaggedMontages.Num() - 1);
		return TaggedMontages[Selection];
	}
	return FTaggedMontage();
}
