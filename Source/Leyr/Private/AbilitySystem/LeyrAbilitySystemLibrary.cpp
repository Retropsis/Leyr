// @ Retropsis 2024-2025.

#include "AbilitySystem/LeyrAbilitySystemLibrary.h"

#include "AbilitySystemComponent.h"
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
	const ALeyrGameMode* LeyrGameMode = Cast<ALeyrGameMode>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (LeyrGameMode == nullptr) return;

	const AActor* AvatarActor = ASC->GetAvatarActor();

	UCharacterClassInfo* CharacterClassInfo = LeyrGameMode->CharacterClassInfo;
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
