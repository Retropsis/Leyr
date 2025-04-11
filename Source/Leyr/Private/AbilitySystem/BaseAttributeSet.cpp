// @ Retropsis 2024-2025.

#include "AbilitySystem/BaseAttributeSet.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilityTypes.h"
#include "GameplayEffectComponents/TargetTagsGameplayEffectComponent.h"
#include "GameplayEffectExtension.h"
#include "AbilitySystem/LeyrAbilitySystemLibrary.h"
#include "Game/BaseGameplayTags.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Interaction/AbilityActorInterface.h"
#include "Interaction/CombatInterface.h"
#include "Interaction/PlayerInterface.h"
#include "Net/UnrealNetwork.h"
#include "Player/PlayerCharacterController.h"

UBaseAttributeSet::UBaseAttributeSet() {}

void UBaseAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Primary Attributes
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, Strength, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, Dexterity, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, Vitality, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, Intelligence, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, Wisdom, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, Spirit, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, Luck, COND_None, REPNOTIFY_Always);
	
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, PhysicalAttackBase, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, MagicalAttackBase, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, PhysicalDefenseBase, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, MagicalDefenseBase, COND_None, REPNOTIFY_Always);
	
	// Secondary Attributes
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, PhysicalAttack, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, MagicalAttack, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, PhysicalDefense, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, MagicalDefense, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, ArmorPenetration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, BlockChance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, CriticalHitChance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, CriticalHitDamage, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, CriticalHitResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, HealthRegeneration, COND_None, REPNOTIFY_Always);	
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, ManaRegeneration, COND_None, REPNOTIFY_Always);	
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);	
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, Poise, COND_None, REPNOTIFY_Always);
	
	// Resistance Attributes
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, PhysicalResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, BluntResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, CuttingResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, PiercingResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, FireResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, IceResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, WindResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, LightningResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, HolyResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, DarkResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, NoxiousResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, ExecutionResistance, COND_None, REPNOTIFY_Always);
	
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, Mana, COND_None, REPNOTIFY_Always);
}

void UBaseAttributeSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::PreAttributeBaseChange(Attribute, NewValue);
	
	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
	}
	if (Attribute == GetManaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxMana());
	}
}

void UBaseAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	
	FEffectProperties Props;
	SetEffectProperties(Data, Props);
	
	if(Props.TargetAvatarActor->Implements<UCombatInterface>() && ICombatInterface::Execute_GetDefeatState(Props.TargetAvatarActor) >= EDefeatState::Defeated) return;
	if(Props.TargetAvatarActor->Implements<UAbilityActorInterface>() && IAbilityActorInterface::Execute_IsDestroyed(Props.TargetAvatarActor)) return;
	// if(Props.TargetCharacter->Implements<UCombatInterface>() && ICombatInterface::Execute_IsDead(Props.TargetCharacter)) return;

	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));
	}
	if (Data.EvaluatedData.Attribute == GetManaAttribute())
	{
		SetMana(FMath::Clamp(GetMana(), 0.f, GetMaxMana()));
	}
	if (Data.EvaluatedData.Attribute == GetIncomingDamageAttribute())
	{
		HandleIncomingDamage(Props);
	}
	if (Data.EvaluatedData.Attribute == GetIncomingHealingAttribute())
	{
		HandleIncomingHealing(Props);
	}
	if (Data.EvaluatedData.Attribute == GetIncomingStatusEffectAttribute())
	{
		HandleIncomingStatusEffect(Props);
	}
	if (Data.EvaluatedData.Attribute == GetIncomingXPAttribute())
	{
		HandleIncomingXP(Props);
	}
}

void UBaseAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);
	
	if (Attribute == GetMaxHealthAttribute() && bRefillHealth)
	{
		SetHealth(GetMaxHealth());
		bRefillHealth = false;
	}
	if (Attribute == GetMaxManaAttribute() && bRefillMana)
	{
		SetMana(GetMaxMana());
		bRefillMana = false;
	}
}

void UBaseAttributeSet::ShowFloatingText(const FEffectProperties& Props, FUIMessageData& MessageData) const
{
	if (Props.SourceCharacter != Props.TargetCharacter)
	{
		if(IsValid(Props.SourceCharacter))
		{
			if(APlayerCharacterController* PC = Cast<APlayerCharacterController>(Props.SourceCharacter->Controller))
			{
				MessageData.MessageType = EMessageType::DamageToEnemy;
				MessageData.TargetActor = Props.TargetAvatarActor;
				PC->ClientShowDamageNumber(MessageData);
				return;
			}
		}
		if(!IsValid(Props.TargetCharacter)) return;
		if(APlayerCharacterController* PC = Cast<APlayerCharacterController>(Props.TargetCharacter->Controller))
		{
			MessageData.MessageType = EMessageType::DamageToPlayer;
			MessageData.TargetActor = Props.TargetAvatarActor;
			PC->ClientShowDamageNumber(MessageData);
		}
	}
	else if(IsValid(Props.TargetCharacter))
	{
		if(APlayerCharacterController* PC = Cast<APlayerCharacterController>(Props.TargetCharacter->Controller))
		{
			MessageData.MessageType = EMessageType::HealingToPlayer;
			MessageData.TargetActor = Props.TargetAvatarActor;
			PC->ClientShowDamageNumber(MessageData);
		}
	}
}

void UBaseAttributeSet::SendXPEvent(const FEffectProperties& Props)
{
	if (Props.TargetAvatarActor->Implements<UCombatInterface>())
	{
		const int32 TargetLevel = ICombatInterface::Execute_GetCharacterLevel(Props.TargetAvatarActor);
		const ECharacterClass TargetClass = ICombatInterface::Execute_GetCharacterClass(Props.TargetAvatarActor);
		const int32 XPReward = ULeyrAbilitySystemLibrary::GetXPRewardForClassAndLevel(Props.TargetAvatarActor, TargetClass, TargetLevel);

		const FBaseGameplayTags& GameplayTags = FBaseGameplayTags::Get();
		FGameplayEventData Payload;
		Payload.EventTag = GameplayTags.Attributes_Meta_IncomingXP;
		Payload.EventMagnitude = XPReward;
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Props.SourceCharacter, GameplayTags.Attributes_Meta_IncomingXP, Payload);
		
		// if(APlayerCharacterController* PC = Cast<APlayerCharacterController>(Props.SourceCharacter->Controller))
		// {
		// 	const FUIMessageData MessageData{ EMessageType::Experience, static_cast<float>(XPReward), Props.TargetAvatarActor, false, false };
		// 	PC->ClientShowDamageNumber(MessageData);
		// }
	}
	if(Props.TargetAvatarActor->Implements<UAbilityActorInterface>())
	{
		//TODO: XP from Actors
	}
}

void UBaseAttributeSet::HandleIncomingDamage(const FEffectProperties& Props)
{
	if(Props.TargetASC && Props.TargetASC->HasMatchingGameplayTag(FBaseGameplayTags::Get().Invincibility)) return;
	
	float LocalIncomingDamage = Props.TargetAvatarActor->Implements<UAbilityActorInterface>() ? 1.f :  GetIncomingDamage();
	SetIncomingDamage(0.f);
	
	if (LocalIncomingDamage > 0.f)
	{		
		const float NewHealth = GetHealth() - LocalIncomingDamage;
		SetHealth(FMath::Clamp(NewHealth, 0.f, GetMaxHealth()));

		if (NewHealth <= 0.f)
		{
			if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(Props.TargetAvatarActor))
			{
				CombatInterface->Die(ULeyrAbilitySystemLibrary::GetDeathImpulse(Props.EffectContextHandle), ULeyrAbilitySystemLibrary::IsExecuteHit(Props.EffectContextHandle));
			}
			if (IAbilityActorInterface* AbilityActorInterface = Cast<IAbilityActorInterface>(Props.TargetAvatarActor))
			{
				AbilityActorInterface->Execute_DestroyActor(Props.TargetAvatarActor);
			}
			SendXPEvent(Props);
		}
		else
		{
			if (Props.TargetCharacter->Implements<UCombatInterface>() && !ICombatInterface::Execute_IsElectrocuted(Props.TargetCharacter))
			{
				Props.TargetASC->TryActivateAbilitiesByTag(FBaseGameplayTags::Get().Effects_HitReact.GetSingleTagContainer());
			}
			const FVector& AirborneForce = ULeyrAbilitySystemLibrary::GetAirborneForce(Props.EffectContextHandle);
			if (!AirborneForce.IsNearlyZero(1.f))
			{
				Props.TargetCharacter->GetCharacterMovement()->StopMovementImmediately();
				Props.TargetCharacter->LaunchCharacter(AirborneForce, true, true);
			}
		}
		
		if(Props.TargetAvatarActor->Implements<UAbilityActorInterface>()) return;

		/* Message */
		const bool bBlockedHit = ULeyrAbilitySystemLibrary::IsBlockedHit(Props.EffectContextHandle);
		const bool bCriticalHit = ULeyrAbilitySystemLibrary::IsCriticalHit(Props.EffectContextHandle);
		FUIMessageData MessageData{ EMessageType::DamageToEnemy, LocalIncomingDamage, Props.TargetAvatarActor, bBlockedHit, bCriticalHit };
		ShowFloatingText(Props, MessageData);

		/* Status Effect */
		if (ULeyrAbilitySystemLibrary::IsSuccessfulStatusEffect(Props.EffectContextHandle))
		{
			HandleStatusEffect(Props);
		}
	}
}

void UBaseAttributeSet::HandleIncomingHealing(const FEffectProperties& Props)
{
	float LocalIncomingHealing = Props.TargetAvatarActor->Implements<UAbilityActorInterface>() ? 1.f :  GetIncomingHealing();
	SetIncomingHealing(0.f);
	
	if (LocalIncomingHealing > 0.f)
	{		
		const float NewHealth = GetHealth() + LocalIncomingHealing;
		SetHealth(FMath::Clamp(NewHealth, 0.f, GetMaxHealth()));
		FUIMessageData MessageData{ EMessageType::HealingToPlayer, LocalIncomingHealing, Props.TargetAvatarActor, false, false };
		ShowFloatingText(Props, MessageData);
	}
}

void UBaseAttributeSet::HandleIncomingStatusEffect(const FEffectProperties& Props)
{
	if (ULeyrAbilitySystemLibrary::IsSuccessfulStatusEffect(Props.EffectContextHandle))
	{
		HandleStatusEffect(Props);
	}
}

void UBaseAttributeSet::HandleIncomingXP(const FEffectProperties& Props)
{
	const float LocalIncomingXP = GetIncomingXP();
	SetIncomingXP(0.f);
		
	if (Props.SourceCharacter->Implements<UPlayerInterface>() && Props.SourceCharacter->Implements<UCombatInterface>())
	{
		const int32 CurrentLevel = ICombatInterface::Execute_GetCharacterLevel(Props.SourceCharacter);
		const int32 CurrentXP = IPlayerInterface::Execute_GetXP(Props.SourceCharacter);

		const int32 NewLevel = IPlayerInterface::Execute_FindLevelForXP(Props.SourceCharacter, CurrentXP + LocalIncomingXP);
		const int32 NumLevelUps = NewLevel - CurrentLevel;
			
		if(NumLevelUps > 0)
		{
			IPlayerInterface::Execute_AddToPlayerLevel(Props.SourceCharacter, NumLevelUps);
			
			int32 AttributePointReward = 0;
			int32 SkillPointReward = 0;

			for (int i = 0; i < NumLevelUps; ++i)
			{
				SkillPointReward += IPlayerInterface::Execute_GetSkillPointsReward(Props.SourceCharacter, CurrentLevel + i);
				AttributePointReward += IPlayerInterface::Execute_GetAttributePointsReward(Props.SourceCharacter, CurrentLevel + i);
			}
			IPlayerInterface::Execute_AddToAttributePoints(Props.SourceCharacter, AttributePointReward);
			IPlayerInterface::Execute_AddToSkillPoints(Props.SourceCharacter, SkillPointReward);

			bRefillHealth = true;
			bRefillMana = true;
				
			IPlayerInterface::Execute_LevelUp(Props.SourceCharacter);
		}
		IPlayerInterface::Execute_AddToXP(Props.SourceCharacter, LocalIncomingXP);
	}
}

void UBaseAttributeSet::HandleStatusEffect(const FEffectProperties& Props)
{
	const FBaseGameplayTags& GameplayTags = FBaseGameplayTags::Get();
	FGameplayEffectContextHandle EffectContext = Props.SourceASC->MakeEffectContext();
	EffectContext.AddSourceObject(Props.SourceAvatarActor);

	const FGameplayTag DamageType = ULeyrAbilitySystemLibrary::GetDamageType(Props.EffectContextHandle);
	const FGameplayTag StatusEffectType = ULeyrAbilitySystemLibrary::GetStatusEffectType(Props.EffectContextHandle);
	const float StatusEffectDamage = ULeyrAbilitySystemLibrary::GetStatusEffectDamage(Props.EffectContextHandle);
	const float StatusEffectDuration = ULeyrAbilitySystemLibrary::GetStatusEffectDuration(Props.EffectContextHandle);
	const float StatusEffectFrequency = ULeyrAbilitySystemLibrary::GetStatusEffectFrequency(Props.EffectContextHandle);

	const FString StatusEffectName = FString::Printf(TEXT("Dynamic_%s"), *StatusEffectType.ToString());
	UGameplayEffect* Effect = NewObject<UGameplayEffect>(GetTransientPackage(), FName(StatusEffectName));
	
	Effect->DurationPolicy = EGameplayEffectDurationType::HasDuration;
	Effect->Period = StatusEffectFrequency;
	Effect->DurationMagnitude = FScalableFloat(StatusEffectDuration);
	Effect->bExecutePeriodicEffectOnApplication = GameplayTags.StatusEffectToPeriodicEffectOnApplicationPolicy[StatusEffectType];
	
	UTargetTagsGameplayEffectComponent& AssetTagsComponent = Effect->FindOrAddComponent<UTargetTagsGameplayEffectComponent>();
	FInheritedTagContainer InheritedTagContainer;
	
	InheritedTagContainer.Added.AddTag(StatusEffectType);
	if (StatusEffectType.MatchesTagExact(GameplayTags.StatusEffect_Stun))
	{
		InheritedTagContainer.Added.AddTag(GameplayTags.Player_Block_CursorTrace);
		InheritedTagContainer.Added.AddTag(GameplayTags.Player_Block_InputHeld);
		InheritedTagContainer.Added.AddTag(GameplayTags.Player_Block_InputPressed);
		InheritedTagContainer.Added.AddTag(GameplayTags.Player_Block_InputReleased);
	}
	AssetTagsComponent.SetAndApplyTargetTagChanges(InheritedTagContainer);
	
	Effect->StackingType = EGameplayEffectStackingType::AggregateBySource;
	Effect->StackLimitCount = 1;

	const int32 Index = Effect->Modifiers.Num();
	Effect->Modifiers.Add(FGameplayModifierInfo());
	FGameplayModifierInfo& ModifierInfo = Effect->Modifiers[Index];

	ModifierInfo.ModifierMagnitude = FScalableFloat(StatusEffectDamage);
	ModifierInfo.ModifierOp = EGameplayModOp::Additive;
	ModifierInfo.Attribute = GetIncomingDamageAttribute();

	if (FGameplayEffectSpec* MutableSpec = new FGameplayEffectSpec(Effect, EffectContext, 1.f))
	{
		FBaseGameplayEffectContext* BaseContext = static_cast<FBaseGameplayEffectContext*>(MutableSpec->GetContext().Get());
		TSharedPtr<FGameplayTag> StatusEffectDamageType = MakeShareable(new FGameplayTag(DamageType));
		BaseContext->SetDamageType(StatusEffectDamageType);

		Props.TargetASC->ApplyGameplayEffectSpecToSelf(*MutableSpec);
	}
}

void UBaseAttributeSet::SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& Props) const
{
	// Source = causer of the effect, Target = target of the effect (owner of this AS)

	Props.EffectContextHandle = Data.EffectSpec.GetContext();
	Props.SourceASC = Props.EffectContextHandle.GetOriginalInstigatorAbilitySystemComponent();

	if (IsValid(Props.SourceASC) && Props.SourceASC->AbilityActorInfo.IsValid() && Props.SourceASC->AbilityActorInfo->AvatarActor.IsValid())
	{
		Props.SourceAvatarActor = Props.SourceASC->AbilityActorInfo->AvatarActor.Get();
		Props.SourceController = Props.SourceASC->AbilityActorInfo->PlayerController.Get();
		if (Props.SourceController == nullptr && Props.SourceAvatarActor != nullptr)
		{
			if (const APawn* Pawn = Cast<APawn>(Props.SourceAvatarActor))
			{
				Props.SourceController = Pawn->GetController();
			}
		}
		if (Props.SourceController)
		{
			Props.SourceCharacter = Cast<ACharacter>(Props.SourceController->GetPawn());
		}
	}

	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		Props.TargetAvatarActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		Props.TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
		Props.TargetCharacter = Cast<ACharacter>(Props.TargetAvatarActor);
		Props.TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Props.TargetAvatarActor);
		Props.TargetASC = &Data.Target;
	}
}

/*
 * Rep Notifies
 */

void UBaseAttributeSet::OnRep_Strength(const FGameplayAttributeData& OldStrength) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, Strength, OldStrength);
}

void UBaseAttributeSet::OnRep_Dexterity(const FGameplayAttributeData& OldDexterity) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, Dexterity, OldDexterity);
}

void UBaseAttributeSet::OnRep_Vitality(const FGameplayAttributeData& OldVitality) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, Vitality, OldVitality);
}

void UBaseAttributeSet::OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, Intelligence, OldIntelligence);
}

void UBaseAttributeSet::OnRep_Wisdom(const FGameplayAttributeData& OldWisdom) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, Wisdom, OldWisdom);
}

void UBaseAttributeSet::OnRep_Spirit(const FGameplayAttributeData& OldSpirit) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, Spirit, OldSpirit);
}

void UBaseAttributeSet::OnRep_Luck(const FGameplayAttributeData& OldLuck) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, Luck, OldLuck);
}

void UBaseAttributeSet::OnRep_Poise(const FGameplayAttributeData& OldPoise) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, Poise, OldPoise);
}

void UBaseAttributeSet::OnRep_PhysicalAttackBase(const FGameplayAttributeData& OldPhysicalAttackBase) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, PhysicalAttackBase, OldPhysicalAttackBase);
}

void UBaseAttributeSet::OnRep_MagicalAttackBase(const FGameplayAttributeData& OldMagicalAttackBase) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, MagicalAttackBase, OldMagicalAttackBase);
}

void UBaseAttributeSet::OnRep_PhysicalDefenseBase(const FGameplayAttributeData& OldPhysicalDefenseBase) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, PhysicalDefenseBase, OldPhysicalDefenseBase);
}

void UBaseAttributeSet::OnRep_MagicalDefenseBase(const FGameplayAttributeData& OldMagicalDefenseBase) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, MagicalDefenseBase, OldMagicalDefenseBase);
}

/* Secondary Attributes Rep Notifies */
void UBaseAttributeSet::OnRep_PhysicalAttack(const FGameplayAttributeData& OldPhysicalAttack) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, PhysicalAttack, OldPhysicalAttack);
}

void UBaseAttributeSet::OnRep_MagicalAttack(const FGameplayAttributeData& OldMagicalAttack) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, MagicalAttack, OldMagicalAttack);
}

void UBaseAttributeSet::OnRep_PhysicalDefense(const FGameplayAttributeData& OldPhysicalDefense) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, PhysicalDefense, OldPhysicalDefense);
}

void UBaseAttributeSet::OnRep_MagicalDefense(const FGameplayAttributeData& OldMagicalDefense) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, MagicalDefense, OldMagicalDefense);
}

void UBaseAttributeSet::OnRep_ArmorPenetration(const FGameplayAttributeData& OldArmorPenetration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, ArmorPenetration, OldArmorPenetration);
}

void UBaseAttributeSet::OnRep_BlockChance(const FGameplayAttributeData& OldBlockChance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, BlockChance, OldBlockChance);
}

void UBaseAttributeSet::OnRep_CriticalHitChance(const FGameplayAttributeData& OldCriticalHitChance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, CriticalHitChance, OldCriticalHitChance);
}

void UBaseAttributeSet::OnRep_CriticalHitDamage(const FGameplayAttributeData& OldCriticalHitDamage) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, CriticalHitDamage, OldCriticalHitDamage);
}

void UBaseAttributeSet::OnRep_CriticalHitResistance(const FGameplayAttributeData& OldCriticalHitResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, CriticalHitResistance, OldCriticalHitResistance);
}

void UBaseAttributeSet::OnRep_HealthRegeneration(const FGameplayAttributeData& OldHealthRegeneration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, HealthRegeneration, OldHealthRegeneration);
}

void UBaseAttributeSet::OnRep_ManaRegeneration(const FGameplayAttributeData& OldManaRegeneration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, ManaRegeneration, OldManaRegeneration);
}

void UBaseAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, MaxHealth, OldMaxHealth);
}

void UBaseAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, MaxMana, OldMaxMana);
}

/*
 * Resistances Rep Notifies
 */
void UBaseAttributeSet::OnRep_PhysicalResistance(const FGameplayAttributeData& OldPhysicalResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, PhysicalResistance, OldPhysicalResistance);
}

void UBaseAttributeSet::OnRep_BluntResistance(const FGameplayAttributeData& OldBluntResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, BluntResistance, OldBluntResistance);
}

void UBaseAttributeSet::OnRep_CuttingResistance(const FGameplayAttributeData& OldCuttingResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, CuttingResistance, OldCuttingResistance);
}

void UBaseAttributeSet::OnRep_PiercingResistance(const FGameplayAttributeData& OldPiercingResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, PiercingResistance, OldPiercingResistance);
}

void UBaseAttributeSet::OnRep_FireResistance(const FGameplayAttributeData& OldFireResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, FireResistance, OldFireResistance);
}

void UBaseAttributeSet::OnRep_IceResistance(const FGameplayAttributeData& OldIceResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, IceResistance, OldIceResistance);
}

void UBaseAttributeSet::OnRep_WindResistance(const FGameplayAttributeData& OldWindResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, WindResistance, OldWindResistance);
}

void UBaseAttributeSet::OnRep_LightningResistance(const FGameplayAttributeData& OldLightningResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, LightningResistance, OldLightningResistance);
}

void UBaseAttributeSet::OnRep_HolyResistance(const FGameplayAttributeData& OldHolyResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, HolyResistance, OldHolyResistance);
}

void UBaseAttributeSet::OnRep_DarkResistance(const FGameplayAttributeData& OldDarkResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, DarkResistance, OldDarkResistance);
}

void UBaseAttributeSet::OnRep_NoxiousResistance(const FGameplayAttributeData& OldNoxiousResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, NoxiousResistance, OldNoxiousResistance);
}

void UBaseAttributeSet::OnRep_ExecutionResistance(const FGameplayAttributeData& OldExecutionResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, ExecutionResistance, OldExecutionResistance);
}

/*
 * Vital Attribute Rep Notifies
 */
void UBaseAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, Health, OldHealth);
}

void UBaseAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, Mana, OldMana);
}