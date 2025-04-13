// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "LeyrAbilitySystemLibrary.generated.h"

struct FStatusEffectParams;
class ULootDataSet;
class UGameplayAbility;
class ULoadMenuSaveGame;
class UInventoryCostData;
class UEquipmentWidgetController;
class UInventoryComponent;
class UContainerComponent;
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
	static void InitializeCharacterAttributesFromSaveData(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, ULoadMenuSaveGame* SaveData);
	
	UFUNCTION(BlueprintCallable, Category="LeyrAbilitySystemLibrary|CharacterClassDefaults", meta=(WorldContext="WorldContextObject"))
	static UCharacterClassInfo* GetCharacterClassInfo(const UObject* WorldContextObject);
	
	UFUNCTION(BlueprintCallable, Category="LeyrAbilitySystemLibrary|CharacterClassDefaults", meta=(WorldContext="WorldContextObject"))
	static UAbilityInfo* GetAbilityInfo(const UObject* WorldContextObject);

	/*
	 * Encounters
	*/
	UFUNCTION(BlueprintCallable, Category="LeyrAbilitySystemLibrary|EncounterInfo", meta=(WorldContext="WorldContextObject"))
	static void InitializeEncounterAttributes(const UObject* WorldContextObject, float Level, UAbilitySystemComponent* ASC);
	
	UFUNCTION(BlueprintCallable, Category="LeyrAbilitySystemLibrary|EncounterInfo", meta=(WorldContext="WorldContextObject"))
	static UEncounterInfo* GetEncounterInfo(const UObject* WorldContextObject);
	
	UFUNCTION(BlueprintCallable, Category="LeyrAbilitySystemLibrary|CharacterClassDefaults", meta=(WorldContext="WorldContextObject"))
	static ULootDataSet* GetLootDataSet(const UObject* WorldContextObject);

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
	
	UFUNCTION(BlueprintCallable, Category = "LeyrAbilitySystemLibrary|GameplayEffects")
	static void SetIsBlockedHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bInIsBlockedHit);

	UFUNCTION(BlueprintCallable, Category = "LeyrAbilitySystemLibrary|GameplayEffects")
	static void SetIsCriticalHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bInIsCriticalHit);

	UFUNCTION(BlueprintPure, Category = "LeyrAbilitySystemLibrary|GameplayEffects")
	static bool IsExecuteHit(const FGameplayEffectContextHandle& EffectContextHandle);
	
	UFUNCTION(BlueprintCallable, Category = "LeyrAbilitySystemLibrary|GameplayEffects")
	static void SetIsExecuteHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bInIsExecuteHit);
	
	UFUNCTION(BlueprintPure, Category = "LeyrAbilitySystemLibrary|GameplayEffects")
	static bool IsRadialDamage(const FGameplayEffectContextHandle& EffectContextHandle);
 
	UFUNCTION(BlueprintPure, Category = "LeyrAbilitySystemLibrary|GameplayEffects")
	static float GetRadialDamageInnerRadius(const FGameplayEffectContextHandle& EffectContextHandle);
 
	UFUNCTION(BlueprintPure, Category = "LeyrAbilitySystemLibrary|GameplayEffects")
	static float GetRadialDamageOuterRadius(const FGameplayEffectContextHandle& EffectContextHandle);
 
	UFUNCTION(BlueprintPure, Category = "LeyrAbilitySystemLibrary|GameplayEffects")
	static FVector GetRadialDamageOrigin(const FGameplayEffectContextHandle& EffectContextHandle);
	
	UFUNCTION(BlueprintCallable, Category = "LeyrAbilitySystemLibrary|GameplayEffects")
	static void SetIsRadialDamage(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bInIsRadialDamage);
 
	UFUNCTION(BlueprintCallable, Category = "LeyrAbilitySystemLibrary|GameplayEffects")
	static void SetRadialDamageInnerRadius(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, float InInnerRadius);
 
	UFUNCTION(BlueprintCallable, Category = "LeyrAbilitySystemLibrary|GameplayEffects")
	static void SetRadialDamageOuterRadius(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, float InOuterRadius);
 
	UFUNCTION(BlueprintCallable, Category = "LeyrAbilitySystemLibrary|GameplayEffects")
	static void SetRadialDamageOrigin(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, const FVector& InOrigin);

	UFUNCTION(BlueprintCallable, Category = "LeyrAbilitySystemLibrary|GameplayEffects")
	static FGameplayEffectContextHandle ApplyAdditionalEffect(const FAdditionalEffectParams& AdditionalEffectParams);
		
	/*
	 * Damage Effect Params
	 */
	UFUNCTION(BlueprintCallable, Category = "LeyrAbilitySystemLibrary|GameplayEffects")
	static void SetIsRadialDamageEffectParam(UPARAM(ref) FAdditionalEffectParams& DamageEffectParams, bool bIsRadial, float InnerRadius, float OuterRadius, FVector Origin);
 
	UFUNCTION(BlueprintCallable, Category = "LeyrAbilitySystemLibrary|GameplayEffects")
	static void SetAirborneDirection(UPARAM(ref) FAdditionalEffectParams& DamageEffectParams, FVector AirborneDirection, float Magnitude = 0.f);
 
	UFUNCTION(BlueprintCallable, Category = "LeyrAbilitySystemLibrary|GameplayEffects")
	static void SetDeathImpulseDirection(UPARAM(ref) FAdditionalEffectParams& DamageEffectParams, FVector ImpulseDirection, float Magnitude = 0.f);
 
	UFUNCTION(BlueprintCallable, Category = "LeyrAbilitySystemLibrary|GameplayEffects")
	static void SetTargetEffectParamsASC(UPARAM(ref) FAdditionalEffectParams& DamageEffectParams, UAbilitySystemComponent* InASC);
	
	/*
	 * Gameplay Mechanics
	 */
	UFUNCTION(BlueprintCallable, Category = "LeyrAbilitySystemLibrary|GameplayMechanics", meta=(WorldContext="WorldContextObject"))
	static void GetLivePlayersWithinRadius(const UObject* WorldContextObject, TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorsToIgnore, float Radius, const FVector& SphereOrigin);
	
	UFUNCTION(BlueprintPure, Category = "LeyrAbilitySystemLibrary|GameplayMechanics")
	static TArray<FRotator> EvenlySpacedRotators(const FVector& Forward, const FVector& Axis, float Spread, int32 NumRotators);
 
	UFUNCTION(BlueprintPure, Category = "LeyrAbilitySystemLibrary|GameplayMechanics")
	static TArray<FVector> EvenlyRotatedVectors(const FVector& Forward, const FVector& Axis, float Spread, int32 NumVectors);
	
	UFUNCTION(BlueprintCallable, Category = "LeyrAbilitySystemLibrary|GameplayMechanics")
	static void GetClosestTargets(int32 MaxTargets, const TArray<AActor*>& Actors, TArray<AActor*>& OutClosestTargets, const FVector& Origin);
	
	UFUNCTION(BlueprintCallable, Category = "LeyrAbilitySystemLibrary|GameplayMechanics")
	static float GetRadialDamageWithFalloff(const AActor* TargetActor, float BaseDamage,  float MinimumDamage, const FVector& Origin, float DamageInnerRadius, float DamageOuterRadius, float DamageFalloff);
	
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
	static FGameplayTag GetStatusEffectType(const FGameplayEffectContextHandle& EffectContextHandle);

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
	static void SetStatusEffectType(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, const FGameplayTag& InStatusEffectType);
	
	UFUNCTION(BlueprintCallable, Category = "LeyrAbilitySystemLibrary|GameplayEffects", meta=(WorldContext="WorldContextObject"))
	static void SetDeathImpulse(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, const FVector& InImpulse);

	UFUNCTION(BlueprintPure, Category = "LeyrAbilitySystemLibrary|GameplayEffects", meta=(WorldContext="WorldContextObject"))
	static void SetAirborneForce(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, const FVector& InForce);
	
	static int32 GetXPRewardForClassAndLevel(const UObject* WorldContextObject, ECharacterClass CharacterClass, int32 CharacterLevel);

	/*
	 * Inventory
	*/
	UFUNCTION(BlueprintPure, Category="LeyrAbilitySystemLibrary|Inventory", meta=(WorldContext="WorldContextObject"))
	static UInventoryComponent* GetContainerComponent(const UObject* WorldContextObject);
	
	UFUNCTION(BlueprintPure, Category="LeyrAbilitySystemLibrary|Inventory", meta=(WorldContext="WorldContextObject"))
	static FInventoryItemData FindItemDataByID(const UObject* WorldContextObject, int32 ItemID);
	
	UFUNCTION(BlueprintPure, Category="LeyrAbilitySystemLibrary|Inventory", meta=(WorldContext="WorldContextObject"))
	static FInventoryItemData FindItemDataByRowName(const UObject* WorldContextObject, FName RowName);
	
	UFUNCTION(BlueprintCallable, Category="LeyrAbilitySystemLibrary|Inventory", meta=(WorldContext="WorldContextObject"))
	static UInventoryCostData* GetInventoryCostData(const UObject* WorldContextObject);
};
