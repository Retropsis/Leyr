// @ Retropsis 2024-2025.

#include "AbilitySystem/LeyrAbilitySystemLibrary.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilityTypes.h"
#include "GameplayEffectTypes.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "AbilitySystem/Data/ActorClassInfo.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "AbilitySystem/Data/EncounterInfo.h"
#include "Data/ItemData.h"
#include "Engine/DamageEvents.h"
#include "Engine/OverlapResult.h"
#include "Game/BaseGameplayTags.h"
#include "Game/LeyrGameMode.h"
#include "Game/LoadMenuSaveGame.h"
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

UCharacterWidgetController* ULeyrAbilitySystemLibrary::GetCharacterWidgetController(const UObject* WorldContextObject)
{
	FWidgetControllerParams WCParams;
	APlayerHUD* PlayerHUD = nullptr;
	if (MakeWidgetControllerParams(WorldContextObject, WCParams, PlayerHUD))
	{
		if(APlayerCharacterController* PC = Cast<APlayerCharacterController>(WCParams.PlayerController))
		{
			return PC->GetCharacterWidgetController(WCParams);
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

void ULeyrAbilitySystemLibrary::InitializeCharacterAttributesFromSaveData(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, ULoadMenuSaveGame* SaveData)
{
	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	if (CharacterClassInfo == nullptr) return;
	
	const FBaseGameplayTags& GameplayTags = FBaseGameplayTags::Get();
	const AActor* SourceAvatarActor = ASC->GetAvatarActor();

	FGameplayEffectContextHandle EffectContextHandle = ASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(SourceAvatarActor);
	const FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->PrimaryAttributes_SetByCaller, 1.f, EffectContextHandle);
	
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Attributes_Primary_Strength, SaveData->Strength);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Attributes_Primary_Dexterity, SaveData->Dexterity);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Attributes_Primary_Vitality, SaveData->Vitality);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Attributes_Primary_Intelligence, SaveData->Intelligence);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Attributes_Primary_Wisdom, SaveData->Wisdom);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Attributes_Primary_Spirit, SaveData->Spirit);
	
	ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data);
	
	FGameplayEffectContextHandle SecondaryAttributesContextHandle = ASC->MakeEffectContext();
	SecondaryAttributesContextHandle.AddSourceObject(SourceAvatarActor);
	const FGameplayEffectSpecHandle SecondaryAttributesSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->SecondaryAttributes_Infinite, 1.f, SecondaryAttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*SecondaryAttributesSpecHandle.Data.Get());

	FGameplayEffectContextHandle VitalAttributesContextHandle = ASC->MakeEffectContext();
	VitalAttributesContextHandle.AddSourceObject(SourceAvatarActor);
	const FGameplayEffectSpecHandle VitalAttributesSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->VitalAttributes, 1.f, VitalAttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*VitalAttributesSpecHandle.Data.Get());
}

void ULeyrAbilitySystemLibrary::InitializeEncounterAttributes(const UObject* WorldContextObject, float Level, UAbilitySystemComponent* ASC)
{	
	const AActor* AvatarActor = ASC->GetAvatarActor();

	UEncounterInfo* EncounterInfo = GetEncounterInfo(WorldContextObject);

	FGameplayEffectContextHandle PrimaryAttributesContextHandle = ASC->MakeEffectContext();
	PrimaryAttributesContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle PrimaryAttributesSpecHandle = ASC->MakeOutgoingSpec(EncounterInfo->PrimaryAttributes, Level, PrimaryAttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*PrimaryAttributesSpecHandle.Data.Get());

	FGameplayEffectContextHandle SecondaryAttributesContextHandle = ASC->MakeEffectContext();
	SecondaryAttributesContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle SecondaryAttributesSpecHandle = ASC->MakeOutgoingSpec(EncounterInfo->SecondaryAttributes, Level, SecondaryAttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*SecondaryAttributesSpecHandle.Data.Get());

	FGameplayEffectContextHandle VitalAttributesContextHandle = ASC->MakeEffectContext();
	VitalAttributesContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle VitalAttributesSpecHandle = ASC->MakeOutgoingSpec(EncounterInfo->VitalAttributes, Level, VitalAttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*VitalAttributesSpecHandle.Data.Get());
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

UAbilityInfo* ULeyrAbilitySystemLibrary::GetAbilityInfo(const UObject* WorldContextObject)
{
	ALeyrGameMode* LeyrGameMode = Cast<ALeyrGameMode>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (LeyrGameMode == nullptr) return nullptr;
	return LeyrGameMode->AbilityInfo;
}

UEncounterInfo* ULeyrAbilitySystemLibrary::GetEncounterInfo(const UObject* WorldContextObject)
{
	ALeyrGameMode* LeyrGameMode = Cast<ALeyrGameMode>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (LeyrGameMode == nullptr) return nullptr;
	return LeyrGameMode->EncounterInfo;
}

ULootDataSet* ULeyrAbilitySystemLibrary::GetLootDataSet(const UObject* WorldContextObject)
{
	ALeyrGameMode* LeyrGameMode = Cast<ALeyrGameMode>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (LeyrGameMode == nullptr) return nullptr;
	return LeyrGameMode->LootDataSet;
}

// void ULeyrAbilitySystemLibrary::UpdateAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, UObject* SourceObject, FGameplayTag InputTag, TArray<FGameplayTag> Abilities)
// {
// 	UAbilityInfo* AbilityInfo = GetAbilityInfo(WorldContextObject);
// 	if(AbilityInfo == nullptr) return;
// 	
// 	const FBaseGameplayTags& GameplayTags = FBaseGameplayTags::Get();
// 	
// 	if(UBaseAbilitySystemComponent* BaseASC = Cast<UBaseAbilitySystemComponent>(ASC))
// 	{
// 		for (FGameplayTag AbilityTag : Abilities)
// 		{
// 			if(FGameplayAbilitySpec* FoundAbilitySpec = BaseASC->GetSpecFromAbilityTag(AbilityTag))
// 			{				
// 				FoundAbilitySpec->DynamicAbilityTags.Reset();
// 				if(AbilityTag.ToString().Contains("Execute"))
// 				{
// 					if (InputTag.MatchesTagExact(GameplayTags.InputTag_LMB))
// 					{
// 						FoundAbilitySpec->DynamicAbilityTags.AddTag(GameplayTags.InputTag_Shoulder_Right);
// 					}
// 					if (InputTag.MatchesTagExact(GameplayTags.InputTag_RMB))
// 					{
// 						FoundAbilitySpec->DynamicAbilityTags.AddTag(GameplayTags.InputTag_Shoulder_Right);
// 					}
// 					if (InputTag.MatchesTagExact(GameplayTags.InputTag_1))
// 					{
// 						FoundAbilitySpec->DynamicAbilityTags.AddTag(GameplayTags.InputTag_Shoulder_Right);
// 					}					
// 				}
// 				else
// 				{
// 					FoundAbilitySpec->DynamicAbilityTags.AddTag(InputTag);
// 				}
// 				ASC->MarkAbilitySpecDirty(*FoundAbilitySpec);
// 				BaseASC->ClientEquipAbility(AbilityTag, GameplayTags.Abilities_Status_Equipped, InputTag, FGameplayTag());
// 			}
// 			else
// 			{
// 				const FBaseAbilityInfo BaseAbilityInfo = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
// 				FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(BaseAbilityInfo.Ability, 1);
// 				if(AbilityTag.ToString().Contains("Execute"))
// 				{
// 					if (InputTag.MatchesTagExact(GameplayTags.InputTag_LMB))
// 					{
// 						AbilitySpec.DynamicAbilityTags.AddTag(GameplayTags.InputTag_Shoulder_Right);
// 					}
// 					if (InputTag.MatchesTagExact(GameplayTags.InputTag_RMB))
// 					{
// 						AbilitySpec.DynamicAbilityTags.AddTag(GameplayTags.InputTag_Shoulder_Right);
// 					}
// 					if (InputTag.MatchesTagExact(GameplayTags.InputTag_1))
// 					{
// 						AbilitySpec.DynamicAbilityTags.AddTag(GameplayTags.InputTag_Shoulder_Right);
// 					}					
// 				}
// 				else
// 				{
// 					AbilitySpec.DynamicAbilityTags.AddTag(InputTag);
// 				}
// 				AbilitySpec.SourceObject = SourceObject;
// 				ASC->GiveAbility(AbilitySpec);
// 				ASC->MarkAbilitySpecDirty(AbilitySpec);
// 				BaseASC->ClientEquipAbility(AbilityTag, GameplayTags.Abilities_Status_Equipped, InputTag, FGameplayTag());
// 			}
// 		}
// 	}
// }

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

bool ULeyrAbilitySystemLibrary::IsRadialDamage(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FBaseGameplayEffectContext* EffectContext = static_cast<const FBaseGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return EffectContext->IsRadialDamage();
	}
	return false;
}

float ULeyrAbilitySystemLibrary::GetRadialDamageInnerRadius(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FBaseGameplayEffectContext* EffectContext = static_cast<const FBaseGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return EffectContext->GetRadialDamageInnerRadius();
	}
	return 0.f;
}

float ULeyrAbilitySystemLibrary::GetRadialDamageOuterRadius(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FBaseGameplayEffectContext* EffectContext = static_cast<const FBaseGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return EffectContext->GetRadialDamageOuterRadius();
	}
	return 0.f;
}

FVector ULeyrAbilitySystemLibrary::GetRadialDamageOrigin(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FBaseGameplayEffectContext* EffectContext = static_cast<const FBaseGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return EffectContext->GetRadialDamageOrigin();
	}
 	return FVector::ZeroVector;
}

void ULeyrAbilitySystemLibrary::SetIsRadialDamage(FGameplayEffectContextHandle& EffectContextHandle, bool bInIsRadialDamage)
{
	if (FBaseGameplayEffectContext* EffectContext = static_cast<FBaseGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		EffectContext->SetIsRadialDamage(bInIsRadialDamage);
	}
}

void ULeyrAbilitySystemLibrary::SetRadialDamageInnerRadius(FGameplayEffectContextHandle& EffectContextHandle, float InInnerRadius)
{
	if (FBaseGameplayEffectContext* EffectContext = static_cast<FBaseGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		EffectContext->SetRadialDamageInnerRadius(InInnerRadius);
	}
}

void ULeyrAbilitySystemLibrary::SetRadialDamageOuterRadius(FGameplayEffectContextHandle& EffectContextHandle, float InOuterRadius)
{
	if (FBaseGameplayEffectContext* EffectContext = static_cast<FBaseGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		EffectContext->SetRadialDamageOuterRadius(InOuterRadius);
	}
}

void ULeyrAbilitySystemLibrary::SetRadialDamageOrigin(FGameplayEffectContextHandle& EffectContextHandle, const FVector& InOrigin)
{
	if (FBaseGameplayEffectContext* EffectContext = static_cast<FBaseGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		EffectContext->SetRadialDamageOrigin(InOrigin);
	}
}

FGameplayEffectContextHandle ULeyrAbilitySystemLibrary::ApplyAdditionalEffect(const FAdditionalEffectParams& AdditionalEffectParams)
{
	const FBaseGameplayTags& GameplayTags = FBaseGameplayTags::Get();
	// const AActor* SourceAvatarActor = AdditionalEffectParams.SourceAbilitySystemComponent->GetAvatarActor();

	FGameplayEffectContextHandle EffectContextHandle = AdditionalEffectParams.SourceAbilitySystemComponent->MakeEffectContext();
	EffectContextHandle.AddSourceObject(AdditionalEffectParams.SourceObject);
	SetDeathImpulse(EffectContextHandle, AdditionalEffectParams.DeathImpulse);
	SetAirborneForce(EffectContextHandle, AdditionalEffectParams.AirborneForce);
	
	SetIsRadialDamage(EffectContextHandle, AdditionalEffectParams.bIsRadialDamage);
	SetRadialDamageInnerRadius(EffectContextHandle, AdditionalEffectParams.RadialDamageInnerRadius);
	SetRadialDamageOuterRadius(EffectContextHandle, AdditionalEffectParams.RadialDamageOuterRadius);
	SetRadialDamageOrigin(EffectContextHandle, AdditionalEffectParams.RadialDamageOrigin);
	
	const FGameplayEffectSpecHandle SpecHandle = AdditionalEffectParams.SourceAbilitySystemComponent->MakeOutgoingSpec(AdditionalEffectParams.AdditionalEffectClass, AdditionalEffectParams.AbilityLevel, EffectContextHandle);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, AdditionalEffectParams.DamageType, AdditionalEffectParams.AbilityPower);

	AdditionalEffectParams.TargetAbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data);
	return EffectContextHandle;
}

// FGameplayEffectContextHandle ULeyrAbilitySystemLibrary::ApplyStatusEffect(const FStatusEffectParams& StatusEffectParams)
// {
// 	const FBaseGameplayTags& GameplayTags = FBaseGameplayTags::Get();
// 	// const AActor* SourceAvatarActor = StatusEffectParams.SourceAbilitySystemComponent->GetAvatarActor();
//
// 	FGameplayEffectContextHandle EffectContextHandle = StatusEffectParams.SourceAbilitySystemComponent->MakeEffectContext();
// 	EffectContextHandle.AddSourceObject(StatusEffectParams.SourceObject);
// 	
// 	const FGameplayEffectSpecHandle SpecHandle = StatusEffectParams.SourceAbilitySystemComponent->MakeOutgoingSpec(StatusEffectParams.AdditionalEffectClass, StatusEffectParams.AbilityLevel, EffectContextHandle);
// 	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, StatusEffectParams.DamageType, StatusEffectParams.AbilityPower);
// 	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, StatusEffectParams.StatusEffectType, StatusEffectParams.AbilityPower);
// 	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.StatusEffect_Chance, StatusEffectParams.StatusEffectChance);
// 	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.StatusEffect_Damage, StatusEffectParams.StatusEffectDamage);
// 	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.StatusEffect_Duration, StatusEffectParams.StatusEffectDuration);
// 	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.StatusEffect_Frequency, StatusEffectParams.StatusEffectFrequency);
//
// 	StatusEffectParams.TargetAbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data);
// 	return EffectContextHandle;
// }

/*
 * Damage Effect Params
 */
void ULeyrAbilitySystemLibrary::SetIsRadialDamageEffectParam(FAdditionalEffectParams& DamageEffectParams, bool bIsRadial, float InnerRadius, float OuterRadius, FVector Origin)
{
	DamageEffectParams.bIsRadialDamage = bIsRadial;
	DamageEffectParams.RadialDamageInnerRadius = InnerRadius;
	DamageEffectParams.RadialDamageOuterRadius = OuterRadius;
	DamageEffectParams.RadialDamageOrigin = Origin;
}

void ULeyrAbilitySystemLibrary::SetAirborneDirection(FAdditionalEffectParams& DamageEffectParams, FVector AirborneDirection, float Magnitude)
{
	AirborneDirection.Normalize();
	if (Magnitude == 0.f)
	{
		DamageEffectParams.AirborneForce = AirborneDirection * DamageEffectParams.AirborneForceMagnitude;
	}
	else
	{
		DamageEffectParams.AirborneForce = AirborneDirection * Magnitude;
	}
}

void ULeyrAbilitySystemLibrary::SetDeathImpulseDirection(FAdditionalEffectParams& DamageEffectParams, FVector ImpulseDirection, float Magnitude)
{
	ImpulseDirection.Normalize();
	if (Magnitude == 0.f)
	{
		DamageEffectParams.DeathImpulse = ImpulseDirection * DamageEffectParams.DeathImpulseMagnitude;
	}
	else
	{
		DamageEffectParams.DeathImpulse = ImpulseDirection * Magnitude;
	}
}

void ULeyrAbilitySystemLibrary::SetTargetEffectParamsASC(FAdditionalEffectParams& DamageEffectParams, UAbilitySystemComponent* InASC)
{
 	DamageEffectParams.TargetAbilitySystemComponent = InASC;
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
			if (Overlap.GetActor()->Implements<UCombatInterface>() && ICombatInterface::Execute_GetDefeatState(Overlap.GetActor()) == EDefeatState::None)
			{
				OutOverlappingActors.AddUnique(ICombatInterface::Execute_GetAvatar(Overlap.GetActor()));
			}
		}
	}
}

TArray<FRotator> ULeyrAbilitySystemLibrary::EvenlySpacedRotators(const FVector& Forward, const FVector& Axis, float Spread, int32 NumRotators)
{
	TArray<FRotator> Rotators;
	const FVector LeftOfSpread = Forward.RotateAngleAxis(-Spread / 2.f, Axis);
	if (NumRotators > 1)
	{
		const float DeltaSpread = Spread / NumRotators;
		for (int32 i = 0; i < NumRotators; i++)
		{
			const FVector Direction = LeftOfSpread.RotateAngleAxis(DeltaSpread * i, Axis);
			Rotators.Add(Direction.Rotation());
		}
	}
	else
	{
		Rotators.Add(Forward.Rotation());
	}
	return Rotators;
}

TArray<FVector> ULeyrAbilitySystemLibrary::EvenlyRotatedVectors(const FVector& Forward, const FVector& Axis, float Spread, int32 NumVectors)
{
 	TArray<FVector> Vectors;
 	const FVector LeftOfSpread = Forward.RotateAngleAxis(-Spread / 2.f, Axis);
 	if (NumVectors > 1)
 	{
 		const float DeltaSpread = Spread / NumVectors;
 		for (int32 i = 0; i < NumVectors; i++)
 		{
 			const FVector Direction = LeftOfSpread.RotateAngleAxis(DeltaSpread * i, FVector::UpVector);
 			Vectors.Add(Direction);
 		}
 	}
 	else
 	{
 		Vectors.Add(Forward);
 	}
 	return Vectors;
}

void ULeyrAbilitySystemLibrary::GetClosestTargets(int32 MaxTargets, const TArray<AActor*>& Actors, TArray<AActor*>& OutClosestTargets, const FVector& Origin)
{
	if (Actors.Num() <= MaxTargets)
	{
		OutClosestTargets = Actors;
		return;
	}
 
	// TArray<AActor*> ActorsToCheck = Actors;
	// int32 NumTargetsFound = 0;
	//
	// while (NumTargetsFound < MaxTargets)
	// {
	// 	if (ActorsToCheck.Num() == 0) break;
	// 	double ClosestDistance = TNumericLimits<double>::Max();
	// 	AActor* ClosestActor;
	// 	for (AActor* PotentialTarget : ActorsToCheck)
	// 	{
	// 		const double Distance = (PotentialTarget->GetActorLocation() - Origin).Length();
	// 		if (Distance < ClosestDistance)
	// 		{
	// 			ClosestDistance = Distance;
	// 			ClosestActor = PotentialTarget;
	// 		}
	// 	}
	// 	ActorsToCheck.Remove(ClosestActor);
	// 	OutClosestTargets.AddUnique(ClosestActor);
	// 	++NumTargetsFound;
	// }

	TArray<AActor*> SortedActors = Actors;
	Algo::Sort(SortedActors, [Origin](const AActor* ActorA, const AActor* ActorB)
	{
		const float DistanceA = FVector::DistSquared(ActorA->GetActorLocation(), Origin);
		const float DistanceB = FVector::DistSquared(ActorB->GetActorLocation(), Origin);
 
		return DistanceA < DistanceB;
	});
 
	for (int32 i = 0; i < MaxTargets; i++)
	{
		OutClosestTargets.Add(SortedActors[i]);
	}
}

float ULeyrAbilitySystemLibrary::GetRadialDamageWithFalloff(const AActor* TargetActor, float BaseDamage,  float MinimumDamage, const FVector& Origin, float DamageInnerRadius, float DamageOuterRadius, float DamageFalloff)
{
	if (!TargetActor) return 0.f;
 
	FRadialDamageParams RadialDamageParams;
	RadialDamageParams.BaseDamage = BaseDamage;
	RadialDamageParams.DamageFalloff = DamageFalloff;
	RadialDamageParams.InnerRadius = DamageInnerRadius;
	RadialDamageParams.OuterRadius = DamageOuterRadius;
	RadialDamageParams.MinimumDamage = MinimumDamage;
	const float DamageScale = RadialDamageParams.GetDamageScale((Origin - TargetActor->GetActorLocation()).Length());
	
	return BaseDamage * DamageScale;
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

FGameplayTag ULeyrAbilitySystemLibrary::GetStatusEffectType(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FBaseGameplayEffectContext* BaseEffectContext = static_cast<const FBaseGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		if (BaseEffectContext->GetStatusEffectType().IsValid())
		{
			return *BaseEffectContext->GetStatusEffectType();
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

void ULeyrAbilitySystemLibrary::SetStatusEffectType(FGameplayEffectContextHandle& EffectContextHandle, const FGameplayTag& InStatusEffectType)
{
	if (FBaseGameplayEffectContext* BaseEffectContext = static_cast<FBaseGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		const TSharedPtr<FGameplayTag> StatusEffectType = MakeShared<FGameplayTag>(InStatusEffectType);
		BaseEffectContext->SetStatusEffectType(StatusEffectType);
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

UInventoryCostData* ULeyrAbilitySystemLibrary::GetInventoryCostData(const UObject* WorldContextObject)
{
	ALeyrGameMode* LeyrGameMode = Cast<ALeyrGameMode>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (LeyrGameMode == nullptr) return nullptr;
	return LeyrGameMode->InventoryCostInfo;
}
