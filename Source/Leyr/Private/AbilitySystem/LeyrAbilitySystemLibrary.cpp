// @ Retropsis 2024-2025.

#include "AbilitySystem/LeyrAbilitySystemLibrary.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilityTypes.h"
#include "GameplayEffectTypes.h"
#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "AbilitySystem/Data/ActorClassInfo.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "AbilitySystem/Data/EncounterInfo.h"
#include "Data/ItemData.h"
#include "Engine/OverlapResult.h"
#include "Game/BaseGameplayTags.h"
#include "Game/LeyrGameMode.h"
#include "GameplayEffectComponents/TargetTagsGameplayEffectComponent.h"
#include "Interaction/CombatInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Player/PlayerCharacter.h"
#include "Player/PlayerCharacterController.h"
#include "Player/PlayerCharacterState.h"
#include "UI/PlayerHUD.h"
#include "UI/Controller/WidgetController.h"
#include "World/Item.h"

/*
 * Widget Controllers
 */
UOverlayWidgetController* ULeyrAbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
	FWidgetControllerParams WCParams;
	APlayerHUD* PlayerHUD = nullptr;
	if(MakeWidgetControllerParams(WorldContextObject, WCParams, PlayerHUD))
	{
		return PlayerHUD->GetOverlayWidgetController(WCParams);
	}
	return nullptr;
}

UAttributeMenuWidgetController* ULeyrAbilitySystemLibrary::GetAttributeMenuWidgetController(const UObject* WorldContextObject)
{
	FWidgetControllerParams WCParams;
	APlayerHUD* PlayerHUD = nullptr;
	if(MakeWidgetControllerParams(WorldContextObject, WCParams, PlayerHUD))
	{
		return PlayerHUD->GetAttributeMenuWidgetController(WCParams);
	}
	return nullptr;
}

USkillMenuWidgetController* ULeyrAbilitySystemLibrary::GetSkillMenuWidgetController(const UObject* WorldContextObject)
{
	FWidgetControllerParams WCParams;
	APlayerHUD* PlayerHUD = nullptr;
	if (MakeWidgetControllerParams(WorldContextObject, WCParams, PlayerHUD))
	{
		return PlayerHUD->GetSkillMenuWidgetController(WCParams);
	}
	return nullptr;
}

UInventoryWidgetController* ULeyrAbilitySystemLibrary::GetInventoryWidgetController(const UObject* WorldContextObject)
{
	FWidgetControllerParams WCParams;
	APlayerHUD* PlayerHUD = nullptr;
	if (MakeWidgetControllerParams(WorldContextObject, WCParams, PlayerHUD))
	{
		if(APlayerCharacterController* PC = Cast<APlayerCharacterController>(WCParams.PlayerController))
		{
			return PC->GetInventoryWidgetController(WCParams);
		}
	}
	return nullptr;
}

UEquipmentWidgetController* ULeyrAbilitySystemLibrary::GetEquipmentWidgetController(const UObject* WorldContextObject)
{
	FWidgetControllerParams WCParams;
	APlayerHUD* PlayerHUD = nullptr;
	if (MakeWidgetControllerParams(WorldContextObject, WCParams, PlayerHUD))
	{
		if(APlayerCharacterController* PC = Cast<APlayerCharacterController>(WCParams.PlayerController))
		{
			return PC->GetEquipmentWidgetController(WCParams);
		}
	}
	return nullptr;
}

bool ULeyrAbilitySystemLibrary::MakeWidgetControllerParams(const UObject* WorldContextObject, FWidgetControllerParams& OutWCParams, APlayerHUD*& OutPlayerHUD)
{
	if (const APlayerController* PC  = WorldContextObject->GetWorld()->GetFirstPlayerController())
	{
		OutPlayerHUD = Cast<APlayerHUD>(PC->GetHUD());
		if(OutPlayerHUD)
		{
			const APlayerCharacterState* PS = PC->GetPlayerState<APlayerCharacterState>();
			
			OutWCParams.PlayerController = WorldContextObject->GetWorld()->GetFirstPlayerController();
			OutWCParams.PlayerState = PC->GetPlayerState<APlayerCharacterState>();
			OutWCParams.AbilitySystemComponent = PS->GetAbilitySystemComponent();
			OutWCParams.AttributeSet = PS->GetAttributeSet();
			return true;
		}
	}
	return false;
}

/*
 *
 */
void ULeyrAbilitySystemLibrary::InitializeCharacterClassAttributes(const UObject* WorldContextObject, ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* ASC)
{
	const AActor* AvatarActor = ASC->GetAvatarActor();

	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	const FCharacterClassDefaultInfo ClassDefaultInfo = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);

	FGameplayEffectContextHandle PrimaryAttributesContextHandle = ASC->MakeEffectContext();
	PrimaryAttributesContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle PrimaryAttributesSpecHandle = ASC->MakeOutgoingSpec(ClassDefaultInfo.PrimaryAttributes, Level, PrimaryAttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*PrimaryAttributesSpecHandle.Data.Get());

	FGameplayEffectContextHandle SecondaryAttributesContextHandle = ASC->MakeEffectContext();
	SecondaryAttributesContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle SecondaryAttributesSpecHandle = ASC->MakeOutgoingSpec(ClassDefaultInfo.SecondaryAttributes, Level, SecondaryAttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*SecondaryAttributesSpecHandle.Data.Get());

	FGameplayEffectContextHandle VitalAttributesContextHandle = ASC->MakeEffectContext();
	VitalAttributesContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle VitalAttributesSpecHandle = ASC->MakeOutgoingSpec(ClassDefaultInfo.VitalAttributes, Level, VitalAttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*VitalAttributesSpecHandle.Data.Get());
}

void ULeyrAbilitySystemLibrary::InitializeEncounterAttributes(const UObject* WorldContextObject, EEncounterName EncounterName, float Level, UAbilitySystemComponent* ASC)
{
	const AActor* AvatarActor = ASC->GetAvatarActor();

	UEncounterInfo* EncounterInfo = GetEncounterInfo(WorldContextObject);
	const FEncounterDefaultInfo EncounterDefaultInfo = EncounterInfo->GetEncounterDefaultInfo(EncounterName);

	FGameplayEffectContextHandle PrimaryAttributesContextHandle = ASC->MakeEffectContext();
	PrimaryAttributesContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle PrimaryAttributesSpecHandle = ASC->MakeOutgoingSpec(EncounterDefaultInfo.PrimaryAttributes, Level, PrimaryAttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*PrimaryAttributesSpecHandle.Data.Get());

	FGameplayEffectContextHandle SecondaryAttributesContextHandle = ASC->MakeEffectContext();
	SecondaryAttributesContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle SecondaryAttributesSpecHandle = ASC->MakeOutgoingSpec(EncounterDefaultInfo.SecondaryAttributes, Level, SecondaryAttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*SecondaryAttributesSpecHandle.Data.Get());

	FGameplayEffectContextHandle VitalAttributesContextHandle = ASC->MakeEffectContext();
	VitalAttributesContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle VitalAttributesSpecHandle = ASC->MakeOutgoingSpec(EncounterDefaultInfo.VitalAttributes, Level, VitalAttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*VitalAttributesSpecHandle.Data.Get());
}

void ULeyrAbilitySystemLibrary::GiveEncounterAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, EEncounterName EncounterName)
{
	UEncounterInfo* EncounterInfo = GetEncounterInfo(WorldContextObject);
	if(EncounterInfo == nullptr) return;
	
	for (const TSubclassOf<UGameplayAbility> AbilityClass : EncounterInfo->CommonAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		ASC->GiveAbility(AbilitySpec);
	}
	const FEncounterDefaultInfo& DefaultInfo = EncounterInfo->GetEncounterDefaultInfo(EncounterName);
	for (TSubclassOf<UGameplayAbility> AbilityClass : DefaultInfo.EncounterAbilities)
	{
		if (ASC->GetAvatarActor()->Implements<UCombatInterface>())
		{
			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, ICombatInterface::Execute_GetCharacterLevel(ASC->GetAvatarActor()));
			ASC->GiveAbility(AbilitySpec);
		}
	}
}

void ULeyrAbilitySystemLibrary::InitializeActorAttributes(const UObject* WorldContextObject, EActorClass ActorClass, float Level, UAbilitySystemComponent* ASC)
{
	const AActor* AvatarActor = ASC->GetAvatarActor();
	
	UActorClassInfo* ActorClassInfo = GetActorClassInfo(WorldContextObject);
	const FActorClassDefaultInfo ClassDefaultInfo = ActorClassInfo->GetClassDefaultInfo(ActorClass);

	FGameplayEffectContextHandle AttributesContextHandle = ASC->MakeEffectContext();
	AttributesContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle AttributesSpecHandle = ASC->MakeOutgoingSpec(ClassDefaultInfo.PrimaryAttributes, Level, AttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*AttributesSpecHandle.Data.Get());

	FGameplayEffectContextHandle VitalAttributesContextHandle = ASC->MakeEffectContext();
	VitalAttributesContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle VitalAttributesSpecHandle = ASC->MakeOutgoingSpec(ClassDefaultInfo.VitalAttributes, Level, VitalAttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*VitalAttributesSpecHandle.Data.Get());
}

void ULeyrAbilitySystemLibrary::GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, ECharacterClass CharacterClass)
{
	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	if(CharacterClassInfo == nullptr) return;
	
	for (const TSubclassOf<UGameplayAbility> AbilityClass : CharacterClassInfo->CommonAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		ASC->GiveAbility(AbilitySpec);
	}
	const FCharacterClassDefaultInfo& DefaultInfo = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);
	for (TSubclassOf<UGameplayAbility> AbilityClass : DefaultInfo.StartupAbilities)
	{
		if (ASC->GetAvatarActor()->Implements<UCombatInterface>())
		{
			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, ICombatInterface::Execute_GetCharacterLevel(ASC->GetAvatarActor()));
			ASC->GiveAbility(AbilitySpec);
		}
	}
}

UCharacterClassInfo* ULeyrAbilitySystemLibrary::GetCharacterClassInfo(const UObject* WorldContextObject)
{
	ALeyrGameMode* LeyrGameMode = Cast<ALeyrGameMode>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (LeyrGameMode == nullptr) return nullptr;
	return LeyrGameMode->CharacterClassInfo;
}

UActorClassInfo* ULeyrAbilitySystemLibrary::GetActorClassInfo(const UObject* WorldContextObject)
{
	ALeyrGameMode* LeyrGameMode = Cast<ALeyrGameMode>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (LeyrGameMode == nullptr) return nullptr;
	return LeyrGameMode->ActorClassInfo;
}

UEncounterInfo* ULeyrAbilitySystemLibrary::GetEncounterInfo(const UObject* WorldContextObject)
{
	ALeyrGameMode* LeyrGameMode = Cast<ALeyrGameMode>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (LeyrGameMode == nullptr) return nullptr;
	return LeyrGameMode->EncounterInfo;
}

UAbilityInfo* ULeyrAbilitySystemLibrary::GetAbilityInfo(const UObject* WorldContextObject)
{
	ALeyrGameMode* LeyrGameMode = Cast<ALeyrGameMode>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (LeyrGameMode == nullptr) return nullptr;
	return LeyrGameMode->AbilityInfo;
}

void ULeyrAbilitySystemLibrary::UpdateMonkAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, FGameplayTag InputTag, bool bShouldClear)
{
	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	if(CharacterClassInfo == nullptr) return;
	
	if(UBaseAbilitySystemComponent* BaseASC = Cast<UBaseAbilitySystemComponent>(ASC))
	{
		for (const FGameplayTag AbilityTag : CharacterClassInfo->MonkAbilities)
		{
			if(FGameplayAbilitySpec* AbilitySpec = BaseASC->GetSpecFromAbilityTag(AbilityTag))
			{				
				const FBaseGameplayTags GameplayTags = FBaseGameplayTags::Get();
				if(bShouldClear)
				{
					AbilitySpec->DynamicAbilityTags.RemoveTag(InputTag);
					BaseASC->ClientEquipAbility(GameplayTags.Abilities_None, GameplayTags.Abilities_None, InputTag, FGameplayTag());
				}
				else
				{
					AbilitySpec->DynamicAbilityTags.AddTag(InputTag);
					BaseASC->ClientEquipAbility(AbilityTag, GameplayTags.Abilities_Status_Equipped, InputTag, FGameplayTag());
				}
				ASC->MarkAbilitySpecDirty(*AbilitySpec);
			}
		}
	}
}

void ULeyrAbilitySystemLibrary::GiveItemAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, FInventoryItemData ItemData, FGameplayTag InputTag)
{
	UAbilityInfo* AbilityInfo = GetAbilityInfo(WorldContextObject);
	if(AbilityInfo == nullptr || ItemData.ID == 0) return;
	
	for (FGameplayTag AbilityTag : ItemData.ItemClass.GetDefaultObject()->GetAbilities())
	{
		const FBaseAbilityInfo BaseAbilityInfo = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
		const FBaseGameplayTags GameplayTags = FBaseGameplayTags::Get();
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(BaseAbilityInfo.Ability, 1);
		FGameplayTagContainer PreviousTags = AbilitySpec.DynamicAbilityTags.Filter(GameplayTags.InputTag.GetSingleTagContainer());
		AbilitySpec.DynamicAbilityTags.RemoveTags(PreviousTags);
		AbilitySpec.DynamicAbilityTags.AddTag(InputTag);
		AbilitySpec.SourceObject = ItemData.Asset.Get();
		ASC->GiveAbility(AbilitySpec);
		ASC->MarkAbilitySpecDirty(AbilitySpec);

		if(UBaseAbilitySystemComponent* BaseASC = Cast<UBaseAbilitySystemComponent>(ASC))
		{
			for (FGameplayTag PreviousTag : PreviousTags)
			{
				BaseASC->ClientEquipAbility(AbilityTag, GameplayTags.Abilities_None, PreviousTag, FGameplayTag());
			}
			BaseASC->ClientEquipAbility(AbilityTag, GameplayTags.Abilities_Status_Equipped, InputTag, FGameplayTag());
		}
	}
}

void ULeyrAbilitySystemLibrary::RemoveItemAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, FInventoryItemData ItemData, FGameplayTag PreviousInputTag)
{
	if(ItemData.ID == 0) return;
	
	for (FGameplayTag AbilityTag : ItemData.ItemClass.GetDefaultObject()->GetAbilities())
	{
		if(UBaseAbilitySystemComponent* BaseASC = Cast<UBaseAbilitySystemComponent>(ASC))
		{
			if(FGameplayAbilitySpec* AbilitySpec = BaseASC->GetSpecFromAbilityTag(AbilityTag))
			{
				AbilitySpec->DynamicAbilityTags.RemoveTag(PreviousInputTag);
				ASC->ClearAbility(AbilitySpec->Handle);
				ASC->MarkAbilitySpecDirty(*AbilitySpec);
				
				const FBaseGameplayTags GameplayTags = FBaseGameplayTags::Get();
				BaseASC->ClientEquipAbility(GameplayTags.Abilities_None, GameplayTags.Abilities_None, PreviousInputTag, FGameplayTag());
			}
		}
	}
}

void ULeyrAbilitySystemLibrary::UpdateItemAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, FInventoryItemData ItemData, FGameplayTag InputTag, bool bShouldClear)
{
	UAbilityInfo* AbilityInfo = GetAbilityInfo(WorldContextObject);
	if(AbilityInfo == nullptr || ItemData.ID == 0) return;
	
	const FBaseGameplayTags GameplayTags = FBaseGameplayTags::Get();
	if(UBaseAbilitySystemComponent* BaseASC = Cast<UBaseAbilitySystemComponent>(ASC))
	{
		for (FGameplayTag AbilityTag : ItemData.ItemClass.GetDefaultObject()->GetAbilities())
        {
			if(bShouldClear)
			{
				if(FGameplayAbilitySpec* AbilitySpec = BaseASC->GetSpecFromAbilityTag(AbilityTag))
				{
					AbilitySpec->DynamicAbilityTags.RemoveTag(InputTag);
					ASC->ClearAbility(AbilitySpec->Handle);
					ASC->MarkAbilitySpecDirty(*AbilitySpec);
					BaseASC->ClientEquipAbility(GameplayTags.Abilities_None, GameplayTags.Abilities_None, InputTag, FGameplayTag());
				}
			}
			else
			{
				const FBaseAbilityInfo BaseAbilityInfo = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
				FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(BaseAbilityInfo.Ability, 1);
				FGameplayTagContainer PreviousTags = AbilitySpec.DynamicAbilityTags.Filter(GameplayTags.InputTag.GetSingleTagContainer());
				AbilitySpec.DynamicAbilityTags.RemoveTags(PreviousTags);
				AbilitySpec.DynamicAbilityTags.AddTag(InputTag);
				AbilitySpec.SourceObject = ItemData.Asset.Get();
				ASC->GiveAbility(AbilitySpec);
				ASC->MarkAbilitySpecDirty(AbilitySpec);
				for (FGameplayTag PreviousTag : PreviousTags)
				{
					BaseASC->ClientEquipAbility(AbilityTag, GameplayTags.Abilities_None, PreviousTag, FGameplayTag());
				}
				BaseASC->ClientEquipAbility(AbilityTag, GameplayTags.Abilities_Status_Equipped, InputTag, FGameplayTag());
			}
        }
	}
	
}

void ULeyrAbilitySystemLibrary::ReplaceAbilityInputTag(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, FInventoryItemData ItemData, FGameplayTag InputTag, FGameplayTag TagToRemove)
{
	if(ItemData.ID == 0) return;
	
	if(UBaseAbilitySystemComponent* BaseASC = Cast<UBaseAbilitySystemComponent>(ASC))
	{
		for (FGameplayTag AbilityTag : ItemData.ItemClass.GetDefaultObject()->GetAbilities())
		{
			if(FGameplayAbilitySpec* AbilitySpec = BaseASC->GetSpecFromAbilityTag(AbilityTag))
			{
				AbilitySpec->DynamicAbilityTags.RemoveTag(TagToRemove);
				AbilitySpec->DynamicAbilityTags.AddTag(InputTag);
						
				const FBaseGameplayTags GameplayTags = FBaseGameplayTags::Get();
				BaseASC->ClientEquipAbility(AbilityTag, GameplayTags.Abilities_Status_Equipped, InputTag, FGameplayTag());
			}
		}
	}
}

bool ULeyrAbilitySystemLibrary::IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FBaseGameplayEffectContext* BaseEffectContext = static_cast<const FBaseGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return BaseEffectContext->IsBlockedHit();
	}
	return false;
}

bool ULeyrAbilitySystemLibrary::IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FBaseGameplayEffectContext* BaseEffectContext = static_cast<const FBaseGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return BaseEffectContext->IsCriticalHit();
	}
	return false;
}

bool ULeyrAbilitySystemLibrary::IsExecuteHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FBaseGameplayEffectContext* BaseEffectContext = static_cast<const FBaseGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return BaseEffectContext->IsExecuteHit();
	}
	return false;
}

void ULeyrAbilitySystemLibrary::SetIsBlockedHit(FGameplayEffectContextHandle& EffectContextHandle, bool bInIsBlockedHit)
{
	if (FBaseGameplayEffectContext* BaseEffectContext = static_cast<FBaseGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		BaseEffectContext->SetIsBlockedHit(bInIsBlockedHit);
	}
}

void ULeyrAbilitySystemLibrary::SetIsCriticalHit(FGameplayEffectContextHandle& EffectContextHandle, bool bInIsCriticalHit)
{
	if (FBaseGameplayEffectContext* BaseEffectContext = static_cast<FBaseGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		BaseEffectContext->SetIsCriticalHit(bInIsCriticalHit);
	}
}

void ULeyrAbilitySystemLibrary::SetIsExecuteHit(FGameplayEffectContextHandle& EffectContextHandle, bool bInIsExecuteHit)
{
	if (FBaseGameplayEffectContext* BaseEffectContext = static_cast<FBaseGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		BaseEffectContext->SetIsExecuteHit(bInIsExecuteHit);
	}
}

FGameplayEffectContextHandle ULeyrAbilitySystemLibrary::ApplyAdditionalEffect(const FAdditionalEffectParams& AdditionalEffectParams)
{
	const FBaseGameplayTags& GameplayTags = FBaseGameplayTags::Get();
	const AActor* SourceAvatarActor = AdditionalEffectParams.SourceAbilitySystemComponent->GetAvatarActor();

	FGameplayEffectContextHandle EffectContextHandle = AdditionalEffectParams.SourceAbilitySystemComponent->MakeEffectContext();
	EffectContextHandle.AddSourceObject(SourceAvatarActor);
	SetDeathImpulse(EffectContextHandle, AdditionalEffectParams.DeathImpulse);
	SetAirborneForce(EffectContextHandle, AdditionalEffectParams.AirborneForce);
	const FGameplayEffectSpecHandle SpecHandle = AdditionalEffectParams.SourceAbilitySystemComponent->MakeOutgoingSpec(AdditionalEffectParams.AdditionalEffectClass, AdditionalEffectParams.AbilityLevel, EffectContextHandle);

	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, AdditionalEffectParams.DamageType, AdditionalEffectParams.AbilityPower);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.StatusEffect_Chance, AdditionalEffectParams.StatusEffectChance);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.StatusEffect_Damage, AdditionalEffectParams.StatusEffectDamage);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.StatusEffect_Duration, AdditionalEffectParams.StatusEffectDuration);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.StatusEffect_Frequency, AdditionalEffectParams.StatusEffectFrequency);

	AdditionalEffectParams.TargetAbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data);
	return EffectContextHandle;
}

/*
 * Gameplay Mechanics
 */
// TODO: Make this utility for 2D Plane
void ULeyrAbilitySystemLibrary::GetLivePlayersWithinRadius(const UObject* WorldContextObject, TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorsToIgnore, float Radius, const FVector& SphereOrigin)
{
	FCollisionQueryParams SphereParams;
	SphereParams.AddIgnoredActors(ActorsToIgnore);

	if (const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		TArray<FOverlapResult> Overlaps;
		World->OverlapMultiByObjectType(Overlaps, SphereOrigin, FQuat::Identity, FCollisionObjectQueryParams(FCollisionObjectQueryParams::InitType::AllDynamicObjects), FCollisionShape::MakeSphere(Radius), SphereParams);
		for (FOverlapResult& Overlap : Overlaps)
		{
			if (Overlap.GetActor()->Implements<UCombatInterface>() && !ICombatInterface::Execute_IsDead(Overlap.GetActor()))
			{
				OutOverlappingActors.AddUnique(ICombatInterface::Execute_GetAvatar(Overlap.GetActor()));
			}
		}
	}
}

bool ULeyrAbilitySystemLibrary::IsHostile(const AActor* FirstActor, const AActor* SecondActor)
{
	return !(FirstActor->ActorHasTag(FName("Player")) && SecondActor->ActorHasTag(FName("Player"))
	|| FirstActor->ActorHasTag(FName("Enemy")) && SecondActor->ActorHasTag(FName("Enemy")));
}

void ULeyrAbilitySystemLibrary::ApplyInvincibilityToTarget(UAbilitySystemComponent* TargetASC, float Duration)
{	
	const FBaseGameplayTags& GameplayTags = FBaseGameplayTags::Get();
	FGameplayEffectContextHandle EffectContext = TargetASC->MakeEffectContext();
	EffectContext.AddSourceObject(TargetASC->GetAvatarActor());

	FString EffectName = FString::Printf(TEXT("Invincibility"));
	UGameplayEffect* Effect = NewObject<UGameplayEffect>(GetTransientPackage(), FName(EffectName));

	Effect->DurationPolicy = EGameplayEffectDurationType::HasDuration;
	Effect->DurationMagnitude = FGameplayEffectModifierMagnitude{ Duration };
	
	UTargetTagsGameplayEffectComponent& AssetTagsComponent = Effect->FindOrAddComponent<UTargetTagsGameplayEffectComponent>();
	FInheritedTagContainer InheritedTagContainer;
	InheritedTagContainer.Added.AddTag(GameplayTags.Invincibility);
	AssetTagsComponent.SetAndApplyTargetTagChanges(InheritedTagContainer);
	
	Effect->StackingType = EGameplayEffectStackingType::None;

	if (FGameplayEffectSpec* MutableSpec = new FGameplayEffectSpec(Effect, EffectContext, 1))
	{
		TargetASC->ApplyGameplayEffectSpecToSelf(*MutableSpec);
	}
}

/*
 * Status Effects
 */
bool ULeyrAbilitySystemLibrary::IsSuccessfulStatusEffect(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FBaseGameplayEffectContext* BaseEffectContext = static_cast<const FBaseGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return BaseEffectContext->IsSuccessfulStatusEffect();
	}
	return false;
}

float ULeyrAbilitySystemLibrary::GetStatusEffectDamage(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FBaseGameplayEffectContext* BaseEffectContext = static_cast<const FBaseGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return BaseEffectContext->GetStatusEffectDamage();
	}
	return 0.f;
}

float ULeyrAbilitySystemLibrary::GetStatusEffectDuration(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FBaseGameplayEffectContext* BaseEffectContext = static_cast<const FBaseGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return BaseEffectContext->GetStatusEffectDuration();
	}
	return 0.f;
}

float ULeyrAbilitySystemLibrary::GetStatusEffectFrequency(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FBaseGameplayEffectContext* BaseEffectContext = static_cast<const FBaseGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return BaseEffectContext->GetStatusEffectFrequency();
	}
	return 0.f;
}

FGameplayTag ULeyrAbilitySystemLibrary::GetDamageType(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FBaseGameplayEffectContext* BaseEffectContext = static_cast<const FBaseGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		if (BaseEffectContext->GetDamageType().IsValid())
		{
			return *BaseEffectContext->GetDamageType();
		}
	}
	return FGameplayTag();
}

FVector ULeyrAbilitySystemLibrary::GetDeathImpulse(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FBaseGameplayEffectContext* BaseEffectContext = static_cast<const FBaseGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return BaseEffectContext->GetDeathImpulse();
	}
	return FVector::ZeroVector;
}

FVector ULeyrAbilitySystemLibrary::GetAirborneForce(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FBaseGameplayEffectContext* BaseEffectContext = static_cast<const FBaseGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return BaseEffectContext->GetAirborneForce();
	}
	return FVector::ZeroVector;
}

void ULeyrAbilitySystemLibrary::SetIsSuccessfulStatusEffect(FGameplayEffectContextHandle& EffectContextHandle, bool bInSuccessfulStatusEffect)
{
	if (FBaseGameplayEffectContext* BaseEffectContext = static_cast<FBaseGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		BaseEffectContext->SetIsSuccessfulStatusEffect(bInSuccessfulStatusEffect);
	}
}

void ULeyrAbilitySystemLibrary::SetStatusEffectDamage(FGameplayEffectContextHandle& EffectContextHandle, float InDamage)
{
	if (FBaseGameplayEffectContext* BaseEffectContext = static_cast<FBaseGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		BaseEffectContext->SetStatusEffectDamage(InDamage);
	}
}

void ULeyrAbilitySystemLibrary::SetStatusEffectDuration(FGameplayEffectContextHandle& EffectContextHandle, float InDuration)
{
	if (FBaseGameplayEffectContext* BaseEffectContext = static_cast<FBaseGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		BaseEffectContext->SetStatusEffectDuration(InDuration);
	}
}

void ULeyrAbilitySystemLibrary::SetStatusEffectFrequency(FGameplayEffectContextHandle& EffectContextHandle, float InFrequency)
{
	if (FBaseGameplayEffectContext* BaseEffectContext = static_cast<FBaseGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		BaseEffectContext->SetStatusEffectFrequency(InFrequency);
	}
}

void ULeyrAbilitySystemLibrary::SetDamageType(FGameplayEffectContextHandle& EffectContextHandle, const FGameplayTag& InDamageType)
{
	if (FBaseGameplayEffectContext* BaseEffectContext = static_cast<FBaseGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		const TSharedPtr<FGameplayTag> DamageType = MakeShared<FGameplayTag>(InDamageType);
		BaseEffectContext->SetDamageType(DamageType);
	}
}

void ULeyrAbilitySystemLibrary::SetDeathImpulse(FGameplayEffectContextHandle& EffectContextHandle, const FVector& InImpulse)
{
	if (FBaseGameplayEffectContext* BaseEffectContext = static_cast<FBaseGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		BaseEffectContext->SetDeathImpulse(InImpulse);
	}
}

void ULeyrAbilitySystemLibrary::SetAirborneForce(FGameplayEffectContextHandle& EffectContextHandle, const FVector& InForce)
{
	if (FBaseGameplayEffectContext* BaseEffectContext = static_cast<FBaseGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		BaseEffectContext->SetAirborneForce(InForce);
	}
}

int32 ULeyrAbilitySystemLibrary::GetXPRewardForClassAndLevel(const UObject* WorldContextObject, ECharacterClass CharacterClass, int32 CharacterLevel)
{
	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	if (CharacterClassInfo == nullptr) return 0;

	const FCharacterClassDefaultInfo& Info = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);
	const float XPReward = Info.XPReward.GetValueAtLevel(CharacterLevel);

	return static_cast<int32>(XPReward);
}

UInventoryComponent* ULeyrAbilitySystemLibrary::GetContainerComponent(const UObject* WorldContextObject)
{
	if (APlayerCharacterController* PCC  = Cast<APlayerCharacterController>(WorldContextObject->GetWorld()->GetFirstPlayerController()))
	{
		return PCC->GetPlayerCharacter() ? PCC->GetPlayerCharacter()->GetInteractingContainer() : nullptr;
	}
	return nullptr;
}

FInventoryItemData ULeyrAbilitySystemLibrary::FindItemDataByID(const UObject* WorldContextObject, int32 ItemID)
{
	ALeyrGameMode* LeyrGameMode = Cast<ALeyrGameMode>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (LeyrGameMode == nullptr) return FInventoryItemData();
	
	return LeyrGameMode->FindItemDataByID(ItemID);
}

FInventoryItemData ULeyrAbilitySystemLibrary::FindItemDataByRowName(const UObject* WorldContextObject, FName RowName)
{
	ALeyrGameMode* LeyrGameMode = Cast<ALeyrGameMode>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (LeyrGameMode == nullptr) return FInventoryItemData();
	
	return LeyrGameMode->FindItemDataByRowName(RowName);
}
