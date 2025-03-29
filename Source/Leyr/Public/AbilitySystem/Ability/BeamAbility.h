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
	
protected:
	UPROPERTY(BlueprintReadWrite, Category="Ability|Damage")
	FVector MouseHitLocation;
 
	UPROPERTY(BlueprintReadWrite, Category="Ability|Damage")
	TObjectPtr<AActor> MouseHitActor;
 
	UPROPERTY(BlueprintReadWrite, Category="Ability|Damage")
	TObjectPtr<APlayerController> OwnerPlayerController;
	
	UPROPERTY(BlueprintReadWrite, Category="Ability|Damage")
	TObjectPtr<ACharacter> OwnerCharacter;
};
