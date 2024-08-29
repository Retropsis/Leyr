// @ Retropsis 2024-2025.

#include "AbilitySystem/LeyrAbilitySystemLibrary.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilityTypes.h"
#include "GameplayEffectTypes.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Engine/OverlapResult.h"
#include "Game/BaseGameplayTags.h"
#include "Game/LeyrGameMode.h"
#include "Interaction/CombatInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Player/PlayerCharacterState.h"
#include "UI/PlayerHUD.h"
#include "UI/Controller/WidgetController.h"

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
void ULeyrAbilitySystemLibrary::InitializeDefaultAttributes(const UObject* WorldContextObject, ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* ASC)
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
	const FGameplayEffectSpecHandle SecondaryAttributesSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->SecondaryAttributes, Level, SecondaryAttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*SecondaryAttributesSpecHandle.Data.Get());

	FGameplayEffectContextHandle VitalAttributesContextHandle = ASC->MakeEffectContext();
	VitalAttributesContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle VitalAttributesSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->VitalAttributes, Level, VitalAttributesContextHandle);
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

UAbilityInfo* ULeyrAbilitySystemLibrary::GetAbilityInfo(const UObject* WorldContextObject)
{
	ALeyrGameMode* LeyrGameMode = Cast<ALeyrGameMode>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (LeyrGameMode == nullptr) return nullptr;
	return LeyrGameMode->AbilityInfo;
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
