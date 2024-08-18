// @ Retropsis 2024-2025.

#include "AbilitySystem/LeyrAbilitySystemLibrary.h"

#include "AbilitySystemComponent.h"
#include "AbilityTypes.h"
#include "GameplayEffectTypes.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Game/LeyrGameMode.h"
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

void ULeyrAbilitySystemLibrary::GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC)
{
	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	if(CharacterClassInfo == nullptr) return;
	
	for (const TSubclassOf<UGameplayAbility> AbilityClass : CharacterClassInfo->CommonAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		ASC->GiveAbility(AbilitySpec);
	}
}

UCharacterClassInfo* ULeyrAbilitySystemLibrary::GetCharacterClassInfo(const UObject* WorldContextObject)
{
	ALeyrGameMode* LeyrGameMode = Cast<ALeyrGameMode>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (LeyrGameMode == nullptr) return nullptr;
	return LeyrGameMode->CharacterClassInfo;
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
