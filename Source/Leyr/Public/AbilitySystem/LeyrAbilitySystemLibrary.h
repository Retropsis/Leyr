// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "LeyrAbilitySystemLibrary.generated.h"

class UAbilityInfo;
class USkillMenuWidgetController;
struct FGameplayEffectContextHandle;
class UCharacterClassInfo;
class UAbilitySystemComponent;
enum class ECharacterClass : uint8;
class UAttributeMenuWidgetController;
class APlayerHUD;
struct FWidgetControllerParams;
class UOverlayWidgetController;

/**
 * 
 */
UCLASS()
class LEYR_API ULeyrAbilitySystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	/*
	 * Widget Controllers
	 */
	UFUNCTION(BlueprintPure, Category="LeyrAbilitySystemLibrary", meta=(WorldContext="WorldContextObject"))
	static UOverlayWidgetController* GetOverlayWidgetController(const UObject* WorldContextObject);
	
	UFUNCTION(BlueprintPure, Category="LeyrAbilitySystemLibrary", meta=(WorldContext="WorldContextObject"))
	static  UAttributeMenuWidgetController* GetAttributeMenuWidgetController(const UObject* WorldContextObject);
	
	UFUNCTION(BlueprintPure, Category="LeyrAbilitySystemLibrary", meta=(WorldContext="WorldContextObject"))
	static  USkillMenuWidgetController* GetSkillMenuWidgetController(const UObject* WorldContextObject);
	
	UFUNCTION(BlueprintPure, Category="LeyrAbilitySystemLibrary", meta=(WorldContext="WorldContextObject"))
	static  bool MakeWidgetControllerParams(const UObject* WorldContextObject, FWidgetControllerParams& OutWCParams, APlayerHUD*& OutPlayerHUD);

	/*
	 * 
	 */
	UFUNCTION(BlueprintCallable, Category="LeyrAbilitySystemLibrary|CharacterClassDefaults", meta=(WorldContext="WorldContextObject"))
	static void InitializeDefaultAttributes(const UObject* WorldContextObject, ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* ASC);
	
	UFUNCTION(BlueprintCallable, Category="LeyrAbilitySystemLibrary|CharacterClassDefaults", meta=(WorldContext="WorldContextObject"))
	static void GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, ECharacterClass CharacterClass);
	
	UFUNCTION(BlueprintCallable, Category="LeyrAbilitySystemLibrary|CharacterClassDefaults", meta=(WorldContext="WorldContextObject"))
	static UCharacterClassInfo* GetCharacterClassInfo(const UObject* WorldContextObject);
	
	UFUNCTION(BlueprintCallable, Category="LeyrAbilitySystemLibrary|CharacterClassDefaults", meta=(WorldContext="WorldContextObject"))
	static UAbilityInfo* GetAbilityInfo(const UObject* WorldContextObject);

	/*
	 * Custom Effect Context
	 */
	UFUNCTION(BlueprintPure, Category = "LeyrAbilitySystemLibrary|GameplayEffects")
	static bool IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category = "LeyrAbilitySystemLibrary|GameplayEffects")
	static bool IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintCallable, Category = "LeyrAbilitySystemLibrary|GameplayEffects")
	static void SetIsBlockedHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bInIsBlockedHit);

	UFUNCTION(BlueprintCallable, Category = "LeyrAbilitySystemLibrary|GameplayEffects")
	static void SetIsCriticalHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bInIsCriticalHit);

	/*
	 * Gameplay Mechanics
	 */
	UFUNCTION(BlueprintCallable, Category = "LeyrAbilitySystemLibrary|GameplayMechanics", meta=(WorldContext="WorldContextObject"))
	static void GetLivePlayersWithinRadius(const UObject* WorldContextObject, TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorsToIgnore, float Radius, const FVector& SphereOrigin);

	UFUNCTION(BlueprintPure, Category = "LeyrAbilitySystemLibrary|GameplayMechanics")
	static bool IsHostile(const AActor* FirstActor, const AActor* SecondActor);
	
	static int32 GetXPRewardForClassAndLevel(const UObject* WorldContextObject, ECharacterClass CharacterClass, int32 CharacterLevel);
};
