// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "LeyrAbilitySystemLibrary.generated.h"

enum class EContainerType : uint8;
class UItemAbilityInfo;
struct FInventoryItemData;
class UInventoryWidgetController;
enum class EActorClass : uint8;
enum class ECharacterClass : uint8;
enum class EEncounterName : uint8;
struct FAdditionalEffectParams;
struct FGameplayEffectContextHandle;
struct FWidgetControllerParams;
class UAbilitySystemComponent;
class UGameplayEffect;
class UEncounterInfo;
class UActorClassInfo;
class UAbilityInfo;
class UCharacterClassInfo;
class UOverlayWidgetController;
class USkillMenuWidgetController;
class UAttributeMenuWidgetController;
class APlayerHUD;

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
	static  UInventoryWidgetController* GetInventoryWidgetController(const UObject* WorldContextObject);
	
	UFUNCTION(BlueprintPure, Category="LeyrAbilitySystemLibrary", meta=(WorldContext="WorldContextObject"))
	static  bool MakeWidgetControllerParams(const UObject* WorldContextObject, FWidgetControllerParams& OutWCParams, APlayerHUD*& OutPlayerHUD);

	/*
	 * Characters
	 */
	UFUNCTION(BlueprintCallable, Category="LeyrAbilitySystemLibrary|CharacterClassDefaults", meta=(WorldContext="WorldContextObject"))
	static void InitializeCharacterClassAttributes(const UObject* WorldContextObject, ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* ASC);
	
	UFUNCTION(BlueprintCallable, Category="LeyrAbilitySystemLibrary|CharacterClassDefaults", meta=(WorldContext="WorldContextObject"))
	static void GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, ECharacterClass CharacterClass);
	
	UFUNCTION(BlueprintCallable, Category="LeyrAbilitySystemLibrary|CharacterClassDefaults", meta=(WorldContext="WorldContextObject"))
	static UCharacterClassInfo* GetCharacterClassInfo(const UObject* WorldContextObject);
	
	UFUNCTION(BlueprintCallable, Category="LeyrAbilitySystemLibrary|CharacterClassDefaults", meta=(WorldContext="WorldContextObject"))
	static UAbilityInfo* GetAbilityInfo(const UObject* WorldContextObject);
	
	UFUNCTION(BlueprintCallable, Category="LeyrAbilitySystemLibrary|CharacterClassDefaults", meta=(WorldContext="WorldContextObject"))
	static void AssignMonkAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, FGameplayTag InputTag);
	
	UFUNCTION(BlueprintCallable, Category="LeyrAbilitySystemLibrary|CharacterClassDefaults", meta=(WorldContext="WorldContextObject"))
	static void ClearInputFromMonkAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, FGameplayTag InputTag);
	
	UFUNCTION(BlueprintCallable, Category="LeyrAbilitySystemLibrary|CharacterClassDefaults", meta=(WorldContext="WorldContextObject"))
	static void GiveItemAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, FInventoryItemData ItemData, FGameplayTag InputTag);
	
	UFUNCTION(BlueprintCallable, Category="LeyrAbilitySystemLibrary|CharacterClassDefaults", meta=(WorldContext="WorldContextObject"))
	static void RemoveItemAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, FInventoryItemData ItemData, FGameplayTag PreviousInputTag);
	
	UFUNCTION(BlueprintCallable, Category="LeyrAbilitySystemLibrary|CharacterClassDefaults", meta=(WorldContext="WorldContextObject"))
	static void ReplaceAbilityInputTag(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, FInventoryItemData ItemData, FGameplayTag InputTag, FGameplayTag TagToRemove);
	
	// UFUNCTION(BlueprintCallable, Category="BaseAbilitySystemLibrary", meta=(WorldContext="WorldContextObject"))
	// static void GiveMonkAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC);
	//
	// UFUNCTION(BlueprintCallable, Category="BaseAbilitySystemLibrary", meta=(WorldContext="WorldContextObject"))
	// static void RemoveMonkAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC);
	
	UFUNCTION(BlueprintCallable, Category="BaseAbilitySystemLibrary", meta=(WorldContext="WorldContextObject"))
	static UItemAbilityInfo* GetItemAbilityInfo(const UObject* WorldContextObject);

	/*
	 * Encounters
	*/
	UFUNCTION(BlueprintCallable, Category="LeyrAbilitySystemLibrary|EncounterInfo", meta=(WorldContext="WorldContextObject"))
	static void InitializeEncounterAttributes(const UObject* WorldContextObject, EEncounterName EncounterName, float Level, UAbilitySystemComponent* ASC);
	
	UFUNCTION(BlueprintCallable, Category="LeyrAbilitySystemLibrary|CharacterClassDefaults", meta=(WorldContext="WorldContextObject"))
	static void GiveEncounterAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, EEncounterName EncounterName);
	
	UFUNCTION(BlueprintCallable, Category="LeyrAbilitySystemLibrary|EncounterInfo", meta=(WorldContext="WorldContextObject"))
	static UEncounterInfo* GetEncounterInfo(const UObject* WorldContextObject);

	/*
	 * Actors
	 */
	UFUNCTION(BlueprintCallable, Category="LeyrAbilitySystemLibrary|ActorClassDefaults", meta=(WorldContext="WorldContextObject"))
	static void InitializeActorAttributes(const UObject* WorldContextObject, EActorClass ActorClass, float Level, UAbilitySystemComponent* ASC);
	
	UFUNCTION(BlueprintCallable, Category="LeyrAbilitySystemLibrary|ActorClassDefaults", meta=(WorldContext="WorldContextObject"))
	static UActorClassInfo* GetActorClassInfo(const UObject* WorldContextObject);
	
	/*
	 * Custom Effect Context
	 */
	UFUNCTION(BlueprintPure, Category = "LeyrAbilitySystemLibrary|GameplayEffects")
	static bool IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category = "LeyrAbilitySystemLibrary|GameplayEffects")
	static bool IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category = "LeyrAbilitySystemLibrary|GameplayEffects")
	static bool IsExecuteHit(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintCallable, Category = "LeyrAbilitySystemLibrary|GameplayEffects")
	static void SetIsBlockedHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bInIsBlockedHit);

	UFUNCTION(BlueprintCallable, Category = "LeyrAbilitySystemLibrary|GameplayEffects")
	static void SetIsCriticalHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bInIsCriticalHit);

	UFUNCTION(BlueprintCallable, Category = "LeyrAbilitySystemLibrary|GameplayEffects")
	static void SetIsExecuteHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bInIsExecuteHit);

	UFUNCTION(BlueprintCallable, Category = "LeyrAbilitySystemLibrary|AdditionalEffect")
	static FGameplayEffectContextHandle ApplyAdditionalEffect(const FAdditionalEffectParams& AdditionalEffectParams);
	
	/*
	 * Gameplay Mechanics
	 */
	UFUNCTION(BlueprintCallable, Category = "LeyrAbilitySystemLibrary|GameplayMechanics", meta=(WorldContext="WorldContextObject"))
	static void GetLivePlayersWithinRadius(const UObject* WorldContextObject, TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorsToIgnore, float Radius, const FVector& SphereOrigin);

	UFUNCTION(BlueprintPure, Category = "LeyrAbilitySystemLibrary|GameplayMechanics")
	static bool IsHostile(const AActor* FirstActor, const AActor* SecondActor);
	
	UFUNCTION(BlueprintCallable, Category = "LeyrAbilitySystemLibrary|GameplayMechanics")
	static void ApplyInvincibilityToTarget(UAbilitySystemComponent* TargetASC, float Duration = 1.f);
	
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
