// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "LeyrAbilitySystemLibrary.generated.h"

struct FAdditionalEffectParams;
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

	UFUNCTION(BlueprintCallable, Category = "LeyrAbilitySystemLibrary|AdditionalEffect")
	static FGameplayEffectContextHandle ApplyAdditionalEffect(const FAdditionalEffectParams& AdditionalEffectParams);
	
	/*
	 * Gameplay Mechanics
	 */
	UFUNCTION(BlueprintCallable, Category = "LeyrAbilitySystemLibrary|GameplayMechanics", meta=(WorldContext="WorldContextObject"))
	static void GetLivePlayersWithinRadius(const UObject* WorldContextObject, TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorsToIgnore, float Radius, const FVector& SphereOrigin);

	UFUNCTION(BlueprintPure, Category = "LeyrAbilitySystemLibrary|GameplayMechanics")
	static bool IsHostile(const AActor* FirstActor, const AActor* SecondActor);
	
	/*
	 *	Status Effects
	 */
	UFUNCTION(BlueprintPure, Category = "LeyrAbilitySystemLibrary|GameplayEffects", meta=(WorldContext="WorldContextObject"))
	static bool IsSuccessfulStatusEffect(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category = "LeyrAbilitySystemLibrary|GameplayEffects", meta=(WorldContext="WorldContextObject"))
	static float GetStatusEffectDamage(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category = "LeyrAbilitySystemLibrary|GameplayEffects", meta=(WorldContext="WorldContextObject"))
	static float GetStatusEffectDuration(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category = "LeyrAbilitySystemLibrary|GameplayEffects", meta=(WorldContext="WorldContextObject"))
	static float GetStatusEffectFrequency(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category = "LeyrAbilitySystemLibrary|GameplayEffects", meta=(WorldContext="WorldContextObject"))
	static FGameplayTag GetDamageType(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category = "LeyrAbilitySystemLibrary|GameplayEffects", meta=(WorldContext="WorldContextObject"))
	static FVector GetDeathImpulse(const FGameplayEffectContextHandle& EffectContextHandle);
	
	UFUNCTION(BlueprintPure, Category = "LeyrAbilitySystemLibrary|GameplayEffects", meta=(WorldContext="WorldContextObject"))
	static FVector GetAirborneForce(const FGameplayEffectContextHandle& EffectContextHandle);
	
	UFUNCTION(BlueprintCallable, Category = "LeyrAbilitySystemLibrary|GameplayEffects", meta=(WorldContext="WorldContextObject"))
	static void SetIsSuccessfulStatusEffect(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bInSuccessfulStatusEffect);

	UFUNCTION(BlueprintCallable, Category = "LeyrAbilitySystemLibrary|GameplayEffects", meta=(WorldContext="WorldContextObject"))
	static void SetStatusEffectDamage(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, float InDamage);

	UFUNCTION(BlueprintCallable, Category = "LeyrAbilitySystemLibrary|GameplayEffects", meta=(WorldContext="WorldContextObject"))
	static void SetStatusEffectDuration(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, float InDuration);

	UFUNCTION(BlueprintCallable, Category = "LeyrAbilitySystemLibrary|GameplayEffects", meta=(WorldContext="WorldContextObject"))
	static void SetStatusEffectFrequency(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, float InFrequency);

	UFUNCTION(BlueprintCallable, Category = "LeyrAbilitySystemLibrary|GameplayEffects", meta=(WorldContext="WorldContextObject"))
	static void SetDamageType(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, const FGameplayTag& InDamageType);
	
	UFUNCTION(BlueprintCallable, Category = "LeyrAbilitySystemLibrary|GameplayEffects", meta=(WorldContext="WorldContextObject"))
	static void SetDeathImpulse(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, const FVector& InImpulse);

	UFUNCTION(BlueprintPure, Category = "LeyrAbilitySystemLibrary|GameplayEffects", meta=(WorldContext="WorldContextObject"))
	static void SetAirborneForce(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, const FVector& InForce);
	
	static int32 GetXPRewardForClassAndLevel(const UObject* WorldContextObject, ECharacterClass CharacterClass, int32 CharacterLevel);
};
