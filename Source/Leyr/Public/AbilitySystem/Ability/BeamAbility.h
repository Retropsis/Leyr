// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/DamageGameplayAbility.h"
#include "BeamAbility.generated.h"

/**
 * 
 */
UCLASS()
class LEYR_API UBeamAbility : public UDamageGameplayAbility
{
	GENERATED_BODY()
	
public:	
	UFUNCTION(BlueprintCallable)
	void StoreMouseDataInfo(const FHitResult& HitResult);
 
	UFUNCTION(BlueprintCallable)
	void StoreOwnerVariables();
	
	UFUNCTION(BlueprintCallable)
	void TraceFirstTarget(const FVector& BeamTargetLocation);
	
	UFUNCTION(BlueprintCallable)
	void StoreFirstTarget();
	
	UFUNCTION(BlueprintCallable)
	void StoreAdditionalTargets(TArray<AActor*>& OutAdditionalTargets);
	
	UFUNCTION(BlueprintImplementableEvent)
	void PrimaryTargetDied(AActor* DeadActor);
 
	UFUNCTION(BlueprintImplementableEvent)
	void AdditionalTargetDied(AActor* DeadActor);
	
	UFUNCTION(BlueprintCallable)
	void ClearPrimaryOnDeathDelegate();
	
	UFUNCTION(BlueprintCallable)
	void ClearAdditionalOnDeathDelegate(AActor* Actor);
	
	UPROPERTY(EditDefaultsOnly, Category="Ability|Damage")
	int32 MaxNumShockTargets = 5;

	UPROPERTY(EditDefaultsOnly, Category="Ability|Damage")
	float LivePlayersWithinRadius = 350.f;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Ability|Damage")
	float Frequency = .1f;
	
protected: 
	UPROPERTY(BlueprintReadWrite, Category="Ability|Damage")
	TObjectPtr<APlayerController> OwnerPlayerController;
	
	UPROPERTY(BlueprintReadWrite, Category="Ability|Damage")
	TObjectPtr<ACharacter> OwnerCharacter;
};
