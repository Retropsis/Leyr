// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AbilityTask_WaitForAssetLoaded.generated.h"

class UGameplayCueDefinition;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAssetLoadedSignature);
/**
 * 
 */
UCLASS()
class LEYR_API UAbilityTask_WaitForAssetLoaded : public UAbilityTask
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="Ability|Tasks", meta = (DisplayName = "WaitForAssetLoaded", HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "true"))
	static UAbilityTask_WaitForAssetLoaded* CreateWaitForAssetLoaded(UGameplayAbility* OwningAbility, TSoftObjectPtr<USoundBase> ImpactSound);

	UPROPERTY(BlueprintAssignable)
	FAssetLoadedSignature AssetLoaded;

private:
	virtual void Activate() override;

	UPROPERTY()
	UGameplayCueDefinition* GameplayCueDefinition;
	
	UPROPERTY()
	TSoftObjectPtr<USoundBase> ImpactSound;
};
