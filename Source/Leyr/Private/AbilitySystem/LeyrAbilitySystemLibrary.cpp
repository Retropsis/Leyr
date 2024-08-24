// @ Retropsis 2024-2025.

#include "AbilitySystem/LeyrAbilitySystemLibrary.h"

#include "AbilitySystemComponent.h"
#include "AbilityTypes.h"
#include "GameplayEffectTypes.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Engine/OverlapResult.h"
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

int32 ULeyrAbilitySystemLibrary::GetXPRewardForClassAndLevel(const UObject* WorldContextObject, ECharacterClass CharacterClass, int32 CharacterLevel)
{
	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	if (CharacterClassInfo == nullptr) return 0;

	const FCharacterClassDefaultInfo& Info = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);
	const float XPReward = Info.XPReward.GetValueAtLevel(CharacterLevel);

	return static_cast<int32>(XPReward);
}
